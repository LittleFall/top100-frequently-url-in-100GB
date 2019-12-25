# LittleHomework
pingcap的小作业

> 题目：
100GBurl文件，使用1GB内存计算出出现次数top100的url和出现的次数
提示：
注意代码可读性，添加必要的注释（英文）
注意代码风格与规范，添加必要的单元测试与文档
注意异常处理，尝试优化性能

目录：
- /doc 存放相关文档
- /data_finder 用于造数据
- /cpp 表示使用c++语言写的项目
- /go 表示使用go语言写的项目
- /test 测试用

环境：
操作系统 ubuntu 18.04
编辑器/同步工具 sublime_text + sublime_merge


生成数据
url 统一资源定位符
包括
1. scheme:表示协议
2. netloc:域名
3. path:路径
4. params:参数
5. query:查询条件，一般都是get请求的url
6. fragment:锚点，用于直接定位页面的下拉位置，跳转到网页的指定位置