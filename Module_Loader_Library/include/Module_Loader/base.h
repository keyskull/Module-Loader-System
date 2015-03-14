#ifndef BASE_H
#define BASE_H
#define WINDOWS_SYSTEM
#define INIT_CACHE_SIZE 100
#define CMD_CACHE 30
#define _EXIT_SUCCESS ((void *)EXIT_SUCCESS)
#define _EXIT_FAILURE ((void *)EXIT_FAILURE)
#define EXIT_TIMEOUT 2
#define _EXIT_TIMEOUT ((void *)EXIT_TIMEOUT)
#define EXIT_MEMORY_ERROR 3
#define _EXIT_MEMORY_ERROR ((void *)EXIT_MEMORY_ERROR)
#include <stdlib.h>
#include <stdio.h>


#if defined(__cplusplus)
extern "C"
{
#endif                          /* __cplusplus */

	typedef const struct _Receipt{
		const time_t create_time;
		const char *const info;
		const void *const used_func;
		const void *const signal;
	}Receipt;
	typedef	const unsigned char Key_data[4];

	/*system function*/

	/*misc function*/
	_declspec(dllexport) int Default_run(void);
	char **const Tokenizer(char *args, const char Keyword);//分词器
	char *Show_Time_CharFormat(time_t time, const char * format);
	char *Get_date(const char * format);

#if defined(__cplusplus)
}                               /* End of extern "C" */
#endif                          /* __cplusplus */
#endif
	/*
	Get_date("%Y/%m/%d %H:%M:%S");
	%a 星期几的简写
	%A 星期几的全称
	%b 月份的简写
	%B 月份的全称
	%c 标准的日期的时间串
	%C 年份的前两位数字
	%d 十进制表示的每月的第几天
	%D 月/天/年
	%e 在两字符域中，十进制表示的每月的第几天
	%F 年-月-日
	%g 年份的后两位数字，使用基于周的年
	%G 年份，使用基于周的年
	%h 简写的月份名
	%H 24小时制的小时
	%I 12小时制的小时
	%j 十进制表示的每年的第几天
	%m 十进制表示的月份
	%M 十时制表示的分钟数
	%n 新行符
	%p 本地的AM或PM的等价显示
	%r 12小时的时间
	%R 显示小时和分钟：hh:mm
	%S 十进制的秒数
	%t 水平制表符
	%T 显示时分秒：hh:mm:ss
	%u 每周的第几天，星期一为第一天 （值从1到7，星期一为1）
	%U 第年的第几周，把星期日作为第一天（值从0到53）
	%V 每年的第几周，使用基于周的年
	%w 十进制表示的星期几（值从0到6，星期天为0）
	%W 每年的第几周，把星期一做为第一天（值从0到53）
	%x 标准的日期串
	%X 标准的时间串
	%y 不带世纪的十进制年份（值从0到99）
	%Y 带世纪部分的十制年份
	%z，%Z 时区名称，如果不能得到时区名称则返回空字符。
	%% 百分号
	*/
