#pragma once
#include "Buffer.h"
#include "StructDefine.h"

#define JOINTNUM 15
#define MAXNUM 25
struct poseRecord{
	int nGood;
	int nBad;
};

class ksPose
{
private:
	//images
	BaseBuf* m_imgRGB;			// buffer 24
	BaseBuf* m_imgBinary;		// buffer 8
	//joint positions
	JOINTPOS m_jointPos[MAXNUM];
	poseRecord m_Record;
public:
	ksPose(void){};
	ksPose(int width, int height){
		m_imgRGB = new Buffer24(width, height, width*3);
		m_imgBinary = new Buffer24( width, height, width*3 );
		for( int i = 0; i < MAXNUM; ++i )
		{
			m_jointPos[i].pos.x = 0.0f;
			m_jointPos[i].pos.y = 0.0f;				
			m_jointPos[i].pos.z = 0.0f;	
			m_jointPos[i].conf = 0.0f;
		}
	}
	~ksPose(void){
		//if( m_imgRGB != NULL ) delete m_imgRGB;
		//if( m_imgBinary != NULL ) delete m_imgBinary;
	};
public:
	BaseBuf *getRGBImg(){return m_imgRGB;};
	BaseBuf *getBinaryImg(){return m_imgBinary;};
	void	setRGBImg( BaseBuf* rgb){
		if( rgb->width() != m_imgRGB->width() ||
			rgb->height() != m_imgRGB->height() )
			return;
		m_imgRGB->copyBuffer( *rgb );
	}

	void	setBinaryImg( BaseBuf* binary ) {
		if( binary->width() != m_imgBinary->width() ||
			binary->height() != m_imgBinary->height() )
			return;
		m_imgBinary->copyBuffer( *binary );
	}
	JOINTPOS getJointPosAt( int idx ) {
		if( idx >= 0 && idx < MAXNUM )
			return m_jointPos[idx];
	};
	void	setJointPosAt( float x, float y, float z, float conf, int idx ){
		m_jointPos[idx].pos.x = x;
		m_jointPos[idx].pos.y = y;
		m_jointPos[idx].pos.z = z;
		m_jointPos[idx].conf = conf;
	}

	void setJointPosAt( JOINTPOS jointPos, int idx ){
		m_jointPos[idx].pos.x = jointPos.pos.x;
		m_jointPos[idx].pos.y = jointPos.pos.y;
		m_jointPos[idx].pos.z = jointPos.pos.z;
		m_jointPos[idx].conf = jointPos.conf;		
	}
	poseRecord getRecord(){return m_Record;};
	void setRecord( int good, int bad ) {m_Record.nGood = good; m_Record.nBad = bad;};
	void AddaRecord( bool isGood ){ 
		if( isGood )
			m_Record.nGood +=1;
		else
			m_Record.nBad +=1;
	}
};

