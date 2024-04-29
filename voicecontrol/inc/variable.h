#ifndef _VARIABLE_H
#define _VARIABLE_H









#define 	MAX2(x,y)	(((x)>=(y))?(x):(y))
#define 	MIN2(x,y)	(((x)<=(y))?(x):(y))
#define 	SIGN(x)		(((x)<=0)?0:1)
#define 	UPDNLMT16(Var,Max,Min)	{(Var)=((Var)>=(Max))?(Max):(Var);(Var)=((Var)<=(Min))?(Min):(Var);}
#define 	UPDNLMT32(Var,Max,Min)	{(Var)=((Var)>=(Max))?(Max):(Var);(Var)=((Var)<=(Min))?(Min):(Var);}
#define 	UPLMT32(Var,Max)	{(Var)=((Var)>=(Max))?(Max):(Var);}
#define 	DNLMT32(Var,Min)	{(Var)=((Var)<=(Min))?(Min):(Var);}



typedef union
{
	u8 u8Byte[4];
	FLOAT f32Data;
}unFourByte;


extern float Read_ADC_data(ADC_TypeDef* ADCx);
extern void adc_init(void);
extern void ADC_Reset(ADC_TypeDef* ADCx);

#endif
