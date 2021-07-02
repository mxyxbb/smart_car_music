#ifndef _OLED_H
#define _OLED_H

#include <stdint.h>

//      x  : 0-127 
//      y  : 0-63
void OLED_Init(void);


//OLED全亮 用于检测坏点
void OLED_Fill(void);

//清屏
void OLED_CLS(void);

//画一个点 左上角为0,0  右下角 127,63
void OLED_PutPixel(unsigned char x,unsigned char y);


//OLED是按字节显示的 一个字节8位，代表8个纵向像素点，
//清除(0,0)-(0,7)8个点 左上角为0,0  右下角 127,63
void OLED_ClrDot(unsigned char x,unsigned char y);

/*!
  * @brief    画矩形
  * @param    x1  : 0-127   左上角坐标（x1,y1）
  * @param    y1  : 0-63
  * @param    x2  : 0-127   右下角坐标（x2，y2）
  * @param    y2  : 0-63
  * @param    gif : 是否开启动画效果
  * @see      OLED_Rectangle(0, 0, 127,62,0);   //绘制矩形
  */
void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);

/*!
  * @brief    写入一组标准ASCII字符串
  * @param    x  : 0-127   
  * @param    y  : 0-7
  * @param    ch : 要显示的字符串
  * @see      OLED_P6x8Str(0,0,"OLED 3.");  //显示字符串
  */
void OLED_P6x8Str(unsigned char x,unsigned char y, char ch[]);

/*!
  * @brief    写入一组标准ASCII字符串
  * @param    x  : 0-127   
  * @param    y  : 0-6
  * @param    ch : 要显示的字符串
  * @note     一个字符高16  OLED高度64  
  * @see      OLED_P8x16Str(0,0,"OLED 3.");  //显示字符串
  */
void OLED_P8x16Str(unsigned char x,unsigned char y, char ch[]);

/*!
  * @brief    输出汉字字符串
  * @param    x  : 0-127   
  * @param    y  : 0-6
  * @param    ch : 要显示的字符串
  * @note     一个字符高16  OLED高度64   字库需要自己制作
  * @see      OLED_P14x16Str(0,4,"北京龙邱");   //显示汉字
  */
void OLED_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);



/*!
  * @brief    显示BMP图片 最大 128×64
  *
  * @param    x0  : 0-127   左上角坐标（x0,y0）
  * @param    y0  : 0-63
  * @param    wide: 1-127   图片宽度
  * @param    high: 1-64    图片高度
  * @param    bmp : 要显示的图片
  *
  * @return   无
  *
  * @note     图片需要用软件转化
  *
  * @see      OLED_Show_BMP(0, 0, 104, 64, longqiu104x64); //画龙邱loge
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_BMP(unsigned char x0,unsigned char y0,unsigned char wide,unsigned char high,const unsigned char * bmp);

/*!
  * @brief    显示龙邱图片128×64
  *
  * @param    无 
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Show_LQLogo();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_LQLogo(void);

/*!
  * @brief    画边线 OV7725显示图像时 可以用这个画边线
  *
  * @param    无 
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Show_Frame80();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_Frame80(void);

/*!
  * @brief    画边线 MT9V034显示图像时 可以用这个画边线
  *
  * @param    无 
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Show_Frame94();
  *
  * @date     2019/6/13 星期四
  */
void OLED_Show_Frame94(void);

/**
  * @brief    OLED 二值化图像显示
  *
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     无
  *
  * @see      OLED_Road(OLEDH, OLEDW, (unsigned char *)Pixle);
  *
  * @date     2019/6/25 星期二
  */
void OLED_Road(unsigned short high, unsigned short wide, unsigned char *Pixle);

/*!
  * @brief    OLED命令
  *
  * @param    cmd ：命令
  *
  * @return   无
  *
  * @note     无
  *
  * @see      内部调用
  *
  * @date     2019/6/13 星期四
  */
void OLED_WrCmd(unsigned char cmd);

//OLED设置点
void OLED_Set_Pos(unsigned char x, unsigned char y);

//OLED 测试函数
void Test_OLED(void);

#endif

