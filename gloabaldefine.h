#ifndef GLOABALDEFINE_H
#define GLOABALDEFINE_H
#define DEBUG_NI
#define setbit(x,y) x|=(1<<y) //置位操作
#define clrbit(x,y) x&=~(1<<y) //清零操作
#define cpare(x,y) ((x&(1<<y))==qPow(2,y)) //比较操作
#define cpare1(x,y) (x&(1<<y))
typedef unsigned short flag_t;
using arrData=qreal[10];
struct basestr{
    int sno;//Positon
    QString name;//base name
    flag_t rstate;//Resistance state
    int dataid;
};
static const QString verisonno="1.1.4";
enum TestType{TCR,Probe,Calibrate};
enum Measureoptions{Lr,Mr,Hr,Nr};
#pragma execution_character_set("utf-8")
#include<QFile>
#include<QDebug>
//全局定义函数及操作
//DEBUG_NI 定义为调试状态，NI SWITCH 和 DMM 及串口提供调试状态
/*V1.0.0 修正：
 * 追加R1~10全选选项
 * 外部测量追加低阻小于10Ω优化选项
 * 调整所有报表格式。
 * 调整查询显示结果界面，使查询结果横向不使用图框
 * 调整外部测量界面，使显示结果全部显示
 * 追加UAC选项
*/
/*V1.0.1 修正:
 * 追加数据查询界面显示最近两天数据自动选项
 *
*/
/*V1.1.0 修正:
 * 修正基片名称显示颜色不正确问题
 * 修正运行过程中TCR测量界面可进入的问题
 * 追加报表可变温度显示
 * 刷新外部测量时，低阻优化和补偿测量取消选中状态
 * 增加修正计算补充，仅对实施修正且高温超过100℃实施修正
 * 增加TCR计算计算值选项
 * 对TCR测试过程中的外部测量禁用提示做出优化

*/
/* V1.1.1:
 * 增加版本显示标示,d表示调试版本呢,b表示修正版本
*/
/* V1.1.2:
 *  增加临时报表单项选择按钮
 *  增加按钮电阻选择图标
*/
/* V1.1.3:
 * 外部测量小数位调整
 * 计算TCR参数错误调整
 * 安定时间显示,自动打印开启
 */
/*
 * V1.1.4
 * 修正外部测量异常中止(延时启用测试相关按钮)
 * 优化查询结果,按时间顺序排序,调整显示宽度

*/

#endif // GLOABALDEFINE_H
