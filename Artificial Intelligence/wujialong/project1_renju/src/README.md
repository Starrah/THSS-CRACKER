# Renju 源代码

IDE：Visual Studio 2019

请尽量在 Release x86 模式下编译运行

## renju

renju 项目实现了人机对弈的AI，使用说明见可执行文件同目录下的 README

## arena

arena 项目用于验证搜索技术和优化的有效性，以及进行两种不同搜索算法的博弈。

- `PK()`：可以选取文档中提到的三个版本的算法中的两个进行机机博弈
- `evaluateHashEffiency()`：对应文档 4.1 节的实验，用于验证 Hash 表对等效分支因子的优化作用
- `evaluateIDSearchEfficiency()`：对应文档 4.2 节的实验，用于验证 迭代加深搜索对等效分支因子的优化作用

详见作业文档。