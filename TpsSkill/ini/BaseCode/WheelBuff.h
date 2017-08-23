#if !defined WHEEL_BUFF_WW
#define WHEEL_BUFF_WW
#include <memory.h>

class WheelBuff
{

public:

	WheelBuff( unsigned int iBuffSize=10240 )
	{
		m_pData = new char[ iBuffSize ];
		m_iDataLen	= iBuffSize;
		m_iStartPos = 0;
		m_iStopPos	= 0;
		m_bFull = false;
	}

	WheelBuff( const WheelBuff& wbuff )
	{
		m_iDataLen	= wbuff.m_iDataLen;
		m_pData = new char[ m_iDataLen ];
		m_iStartPos = wbuff.m_iStartPos;
		m_iStopPos	= wbuff.m_iStopPos;
		m_bFull = wbuff.m_bFull;
		m_bFull = false;
	}

	virtual ~WheelBuff()
	{
		delete[] m_pData;
	}

	
	/** 写入数据 pData为数据指针 iDataLen为要写入的数据长度 **/
	inline bool PutData( unsigned char *pData, int iDataLen )
	{
		if ( iDataLen == 0 )
		{
			return true;
		}

		int len = 0;
		if ( m_iStopPos == m_iStartPos )
		{
			if ( m_bFull )
			{
				return false;
			}
			else 
			{
				len = 0;
			}
		}
		else if ( m_iStopPos < m_iStartPos )
		{
			len = m_iStopPos + m_iDataLen - m_iStartPos;
		}
		else
		{
			len = m_iStopPos - m_iStartPos;
		}

		int left = m_iDataLen - len;

		if ( left < iDataLen )
		{
			return false;
		}

		if ( m_iStopPos < m_iStartPos )
		{
			memcpy( m_pData+m_iStopPos, pData, iDataLen );
			m_iStopPos += iDataLen;
		}
		else if ( m_iStopPos >= m_iStartPos )
		{
			int tail = m_iDataLen - m_iStopPos;
			if ( tail >= iDataLen )
			{
				memcpy( m_pData+m_iStopPos, pData, iDataLen );
			}
			else
			{
				memcpy( m_pData+m_iStopPos, pData, tail );
				memcpy( m_pData, pData+tail, iDataLen-tail );
			}
			m_iStopPos += iDataLen;
			m_iStopPos %= m_iDataLen;
		}

		if ( m_iStopPos == m_iStartPos )
			m_bFull = true;
		return true;
	}

	inline bool SkipFrontData( unsigned int skipLen )
	{
		if ( skipLen == 0 )
			return true;
		unsigned int len = 0;
		if ( m_iStopPos == m_iStartPos )
		{
			if ( m_bFull )
				len = m_iDataLen;
			else 
				return false;
		}
		else if ( m_iStopPos < m_iStartPos )
		{
			len = m_iStopPos + m_iDataLen - m_iStartPos;
		}
		else
		{
			len = m_iStopPos - m_iStartPos;
		}

		if ( len < skipLen )
			return false;
		m_bFull = false;
		m_iStartPos += skipLen;
		m_iStartPos %= m_iDataLen;
		return true;
	}
	
	/** 获得数据 pData数据缓冲区指针 iDataLen为要读取的数据长度 **/
	inline bool GetData(unsigned char *pData, unsigned int iDataLen, bool bSkip = true )
	{
		if ( iDataLen == 0 )
			return true;
		unsigned int len = 0;
		if ( m_iStopPos == m_iStartPos )
		{
			if ( m_bFull )
				len = m_iDataLen;
			else 
				return false;
		}
		else if ( m_iStopPos < m_iStartPos )
		{
			len = m_iStopPos + m_iDataLen - m_iStartPos;
		}
		else
		{
			len = m_iStopPos - m_iStartPos;
		}

		if ( len < iDataLen )
			return false;
		m_bFull = false;

		if ( m_iStopPos <= m_iStartPos )
		{
			unsigned int tail = m_iDataLen - m_iStartPos;
			if ( tail >= iDataLen )
			{
				memcpy( pData, m_pData+m_iStartPos, iDataLen );
			}
			else
			{
				memcpy( pData, m_pData+m_iStartPos, tail );
				memcpy( pData+tail, m_pData, iDataLen-tail );
			}
			if ( bSkip )
			{
				m_iStartPos += iDataLen;
				m_iStartPos %= m_iDataLen;
			}
		}
		else if ( m_iStopPos > m_iStartPos )
		{
			memcpy( pData, m_pData+m_iStartPos, iDataLen );
			if ( bSkip )
				m_iStartPos += iDataLen;
		}
		return true;
	}

	/** 获得缓冲区内数据的长度 **/
	inline unsigned int GetDataLen()
	{
		unsigned int len = 0;
		if ( m_iStopPos == m_iStartPos )
		{
			if ( m_bFull )
				return m_iDataLen;
			else 
				return 0;
		}
		else if ( m_iStopPos < m_iStartPos )
		{
			len = m_iStopPos + m_iDataLen - m_iStartPos;
		}
		else
		{
			len = m_iStopPos - m_iStartPos;
		}
		return len;
	}

	inline int GetDataLeft()
	{
		return m_iDataLen - GetDataLen();
	}

	inline int GetBuffSize()
	{
		return m_iDataLen;
	}

	void Clear()
	{
		m_iStartPos = 0;
		m_iStopPos	= 0;
		m_bFull = false;
	}

	inline void Resize( unsigned int iBuffSize )
	{
		Clear();
		delete[] m_pData;
		m_pData = new char[ iBuffSize ];
		m_iDataLen = iBuffSize;
	}

	bool getLine( char* buff, int32 len )
	{
		if ( len >= GetDataLeft() )
			return false;
		for ( int32 i=0; i<=len; i++ )
		{
			buff[i] = m_pData[m_iStartPos+i%m_iDataLen];
			if ( buff[i] == '\n' )
			{
				if ( i>0 && buff[i-1] == '\r' )
					buff[i-1] = 0;
				else
					buff[i] = 0;
				m_iStartPos  = (m_iStartPos+i+1)%m_iDataLen;
				return true;
			}
		}
		return false;
	}

	unsigned int		m_iStartPos;		//缓冲区开始位置
	
private:

	char*				m_pData;			//内部缓冲区指针
	unsigned int		m_iDataLen;			//缓冲区数据长度
	
	unsigned int		m_iStopPos;			//缓冲区结束位置
	unsigned int		m_bFull;			//缓冲区是否满标志位
	
};
#endif
