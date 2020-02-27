# 曲线生成算法

## 依赖
- win10 (linux mac需要自己配置环境)
- MingGW compiler (32位版本)
- Visual Studio Code
- opengl(glad, glfw), glm

## 已实现功能
- 可编辑bezier曲线和B样条曲线
- 可控制的行值点
- 条状带的生成与修改
- 条状带的反走样
- 条状带拼接

## 待实现功能
- 更多图元
- 图元的表示与存储

## 运行指南
1. 安装VScode
2. 安装MingGW（32位版本），将MingGW安装位置的bin文件夹路径加入到环境变量PATH中，官网网速不好，推荐直接安装包含MingGW的CodeBlocks
3. git clone 本仓库到本地
4. 用VScode打开项目文件夹，菜单Terminal->Configure Default Build Task，选择Build&run。
5. Ctrl+Shift+B编译运行

## 代码说明
- include/utils 包含工具头文件
- include/modes 包含绘制模式的父类和特定绘制模型的子类
- src/main.cpp 包含主逻辑
  
## 操作方法
- 鼠标右键可以在鼠标位置新建控制点，继续用右键可以继续添加到同一条曲线，鼠标左键结束当前曲线的点插入
- 鼠标左键可以拖动现有的控制点，如果拖动使两条曲线的端点重合，两条曲线会合并为一条
![](imgs/show.gif)
