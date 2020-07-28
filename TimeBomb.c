//���趨��ʱը������
//���ܣ�
//��S6~13ѡ��ʱ�䣬�ֱ�Ϊ30��60��90��120��150��180��210��240��
//ֻ�谴һ��ʱ��ѡ��������S2�����������Զ�����ʱ
//����ʱ������ը�����ڰ�ȫ��������������������Σ��


#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit BEEP = P2^3;//�������͵�ƽ��Ч
sbit WE = P2^7;//�����λѡ
sbit DU = P2^6;//����ܶ�ѡ
uchar KeyValue = 0;//����ֵ


//��������ܶ�ѡ��0-9
uchar  code tabel[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};


void delay(uint z)//���뼶�����ʱ
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 


//����ܶ�̬��ʾ�����������ʾ255
void display(uchar i)
{
	uchar bai, shi, ge;
	bai = i / 100; //236 / 100  = 2
	shi = i % 100 / 10;	//236 % 100 / 10 = 3
	ge  = i % 10;//236 % 10 =6
	
	//��һλ�����  		
	P0 = 0XFF;//�������
	WE = 1;//��λѡ������
	P0 = 0XFE; //1111 1110
	WE = 0;//����λѡ����
	
	DU = 1;//�򿪶�ѡ������
	P0 = tabel[bai];//
	DU = 0;//�����ѡ����
	delay(5);

	//�ڶ�λ�����
	P0 = 0XFF;//�������
	WE = 1;//��λѡ������
	P0 = 0XFD; //1111 1101
	WE = 0;//����λѡ����
	
	DU = 1;//�򿪶�ѡ������
	P0 = tabel[shi];//
	DU = 0;//�����ѡ����
	delay(5);

	//����λ�����
	P0 = 0XFF;//�������
	WE = 1;//��λѡ������
	P0 = 0XFB; //1111 1011
	WE = 0;//����λѡ����
	
	DU = 1;//�򿪶�ѡ������
	P0 = tabel[ge];//
	DU = 0;//�����ѡ����
	delay(5);
}

//����ɨ�躯������ֵ�����Ҵ��ϵ��´�0��ʼ��1����
void KeyScan()
{
	//4*4�������ɨ��
	P3 = 0XF0;//��ɨ��
	if(P3 != 0XF0)//�жϰ����Ƿ񱻰���
	{
		delay(10);//�������10ms
		if(P3 != 0XF0)//�жϰ����Ƿ񱻰���
		{
			switch(P3) //�ж���һ�б�����
			{
				case 0xe0:	KeyValue = 0;	break;//��һ�б�����
				case 0xd0:	KeyValue = 1;	break;//�ڶ��б�����
				case 0xb0:	KeyValue = 2;	break;//�����б�����
				case 0x70:	KeyValue = 3;	break;//�����б����� 
			}
			P3 = 0X0F;//��ɨ��
			switch(P3) //�ж���һ�б�����
			{
				case 0x0e:	KeyValue = KeyValue;	break;//��һ�б�����
				case 0x0d:	KeyValue = KeyValue + 4;	break;//�ڶ��б�����
				case 0x0b:	KeyValue = KeyValue + 8;	break;//�����б�����
				case 0x07:	KeyValue = KeyValue + 12;	break;//�����б����� 
			}
			while(P3 != 0X0F);//���ּ��	
		}
	}
	P3 = 0XFF;//��������ɨ��
	if(P3 != 0XFF)
	{
		delay(10);//�������10ms
		if(P3 != 0XFF)
		{
			switch(P3) //�ж���һ�б�����
			{
				case 0xfe:	KeyValue = 16;	break;//S2������
				case 0xfd:	KeyValue = 17;	break;//S3������
				case 0xfb:	KeyValue = 18;	break;//S4������
				case 0xf7:	KeyValue = 19;	break;//S5������ 
			}
			while(P3 != 0XFF);//���ּ��			
		}	
	}
}


//��ʱ��0��ʼ��
void timer0Init()
{
	TR0 = 1;	 //������ʱ��0
	TMOD = 0X01; //��ʱ������ģʽ1��16λ��ʱ������ģʽ
	TH0 = 0x4b;//(65535-46082)/256 �߰�λ
	TL0 = 0xfd; //(65535-46082)%256 ��ʱ50ms,50ms=50000��s 50000/1.085=46082 �Ͱ�λ
}

void main()//�����������𵹼�ʱ���������ʾ����������ʾ
{	
	uint TimeInput = 0;//�������밴��ֵ
	uchar mSec, Sec;//������봢�����
		while(KeyValue!=16)
		{
				
					KeyScan();
					switch(KeyValue)
					{
						case 0:TimeInput = 30; break;//�����
						case 1:TimeInput = 60; break;//һ����
						case 2:TimeInput = 90; break;//һ�ְ�
						case 3:TimeInput = 120; break;//������
						case 4:TimeInput = 150; break;//���ְ�
						case 5:TimeInput = 180; break;//������
						case 6:TimeInput = 210; break;//���ְ�
						case 7:TimeInput = 240; break;//�ķ���
						default:KeyValue = 16;//ǿ��S2�Կ�ʼ����ʱ
					}
					display(TimeInput);
				
		}
	KeyValue = 16;
	


	
	
	/*��Ҫ���̣�����ʱ*/
				
	timer0Init();//��ʱ��0��ʼ��
	while(1)
	{
		if(TF0 == 1)//�ж��Ƿ����
		{
			TF0 = 0;//������������־λ
			TH0 = 0x4b;
			TL0 = 0xfd; //��ʱ50ms
			mSec++;//50ms��
			if(mSec == 20)
			{
				mSec = 0;
				Sec++;//1��ʱ�䵽
				BEEP = 0;
				delay(50);
				BEEP = ~BEEP;
			}					
		}
		display(TimeInput-Sec); //�������ʾ����

		if(Sec > TimeInput)//����ʱ����
		{
			Sec = 0;//������ 
			BEEP = 0;//����
						
			WE = 1;//��λѡ������
			P0 = 0X00; //ѡͨ���������
			WE = 0;//����λѡ����

			DU = 1;//�򿪶�ѡ������
			P0 = 0Xff;//ȫ��
			DU = 0;//�����ѡ����
						
			while(1);
		}
	}	
}  