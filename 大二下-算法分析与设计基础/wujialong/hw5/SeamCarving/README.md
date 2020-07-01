# 基于接缝裁剪的图片压缩算法

使用 Python 语言实现，本机的实验环境为 Python 3.7.6，安装

```
numpy=1.18.1
opencv=4.2.0
tqdm=4.43.0
```

复现实验结果请尽量安装不低于这些版本的（或接近这些版本的）Python 和包。

## 运行

参数如下：

 - `--image` or `-i`: Path to image. required
 - `--scale_x` or `-x`: Scale to width, must < 1, default = 0.5
 - `--scale_y` or `-y`: Scale to height, must < 1, default = 0.5

输出将位于输入图像同一目录下，文件名后缀 `_output_cols` 和 `_output` 分别表示裁剪完宽度，再裁剪完高度的结果。

如

```bash
python carve.py -i demo/tower.png -x 0.4 -y 0.8
```

会将宽度裁剪为原来的 0.4，高度裁剪为原来的 0.6。

**注：请使用3通道RGB图像进行测试，若使用灰度图像可能无法正常运行**

## 效率

裁剪 320x240 的图片到原尺寸的一半，在实验环境下大约需要 2 min，请尽量使用不高于该尺寸的图片进行测试。