#ifndef __SWM166_DIV_H__
#define __SWM166_DIV_H__


void DIV_Init(DIV_TypeDef * DIVx);


/****************************************************************************************************************************************** 
* 函数名称:	DIV_UDiv()
* 功能说明:	使用硬件除法器执行无符号数除法运算
* 输    入: uint32_t dividend		被除数
*			uint32_t divisor		除数
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE void DIV_UDiv(uint32_t dividend, uint32_t divisor)
{
	DIV->DIVIDEND = dividend;
	DIV->DIVISOR = divisor;
	
	DIV->CR = (1 << DIV_CR_DIVSIGN_Pos) | (1 << DIV_CR_DIVGO_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_SDiv()
* 功能说明:	使用硬件除法器执行有符号数除法运算
* 输    入: int32_t dividend		被除数
*			int32_t divisor			除数
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE void DIV_SDiv(int32_t dividend, int32_t divisor)
{
	DIV->DIVIDEND = dividend;
	DIV->DIVISOR = divisor;
	
	DIV->CR = (0 << DIV_CR_DIVSIGN_Pos) | (1 << DIV_CR_DIVGO_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_Div_IsBusy()
* 功能说明:	硬件除法器是否正在执行除法运算
* 输    入: 无
* 输    出: uint32_t		1 硬件除法器正在执行除法运算    0 硬件除法器已完成除法运算，可取出运算结果
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t DIV_Div_IsBusy(void)
{
	return (DIV->SR & DIV_SR_DIVBUSY_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_UDiv_Result()
* 功能说明:	取出硬件除法器的运算结果
* 输    入: 无
* 输    出: uint32_t *quotient		商
*			uint32_t *remainder		余数
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE void DIV_UDiv_Result(uint32_t *quotient, uint32_t *remainder)
{
	*quotient = DIV->QUO;
	*remainder = DIV->REMAIN;
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_SDiv_Result()
* 功能说明:	取出硬件除法器的运算结果
* 输    入: 无
* 输    出: int32_t *quotient		商
*			int32_t *remainder		余数
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE void DIV_SDiv_Result(int32_t *quotient, int32_t *remainder)
{
	*quotient = DIV->QUO & 0x7FFFFFFF;
	if(DIV->QUO & (1u << 31)) *quotient = 0 - *quotient;
	
	*remainder = DIV->REMAIN & 0x7FFFFFFF;
	if(DIV->REMAIN & (1u << 31)) *remainder = 0 - *remainder;
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_Root()
* 功能说明:	使用硬件开方模块执行开方运算
* 输    入: uint32_t radicand		被开方数
*			uint32_t calcu_frac		0 开方结果为16位整数    1 开方结果为16位整数+16位小数
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE void DIV_Root(uint32_t radicand, uint32_t calcu_fractional)
{
	DIV->RADICAND = radicand;
	
	DIV->CR = (1 << DIV_CR_ROOTGO_Pos) | (calcu_fractional << DIV_CR_ROOTMOD_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_Root_IsBusy()
* 功能说明:	硬件开方模块是否正在执行开方运算
* 输    入: 无
* 输    出: uint32_t		1 硬件开方模块正在执行开方运算    0 硬件开方模块已完成开方运算，可取出运算结果
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t DIV_Root_IsBusy(void)
{
	return (DIV->SR & DIV_SR_ROOTBUSY_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* 函数名称:	DIV_Root_Result()
* 功能说明:	取出硬件开方模块的运算结果
* 输    入: 无
* 输    出: uint32_t		开方结果
* 注意事项: 无
******************************************************************************************************************************************/
static __INLINE uint32_t DIV_Root_Result(void)
{
	if(DIV->CR & DIV_CR_ROOTMOD_Msk)
	{
		return DIV->ROOT;
	}
	else
	{
		return DIV->ROOT >> 16;
	}
}

#endif //__SWM166_DIV_H__
