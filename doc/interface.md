# 算法与软件接口对齐文档

引言：算法的传入接口为字符数组格式，传出接口为JSON格式，为方便开发工作，接口格式记录于本文档，如有修改请及时更新。

## 一 定位算法

**算法传入：**

算法的传入信息包括协议头、图像JSON数据、数据中间分界、base64格式的图像数据与协议尾，如下：

```
{-ALGOHead-}图像JSON数据{-ALGOBound-}图像数据{-ALGOTail-}
```

其中，{-ALGOHead-}为协议头，{-ALGOBound-}为数据中间分界，{-ALGOTail-}为协议尾，图像JSON数据包括如下内容：

```
-imageName
-imageWidth
-imageHeight
-bottomNum
```

**算法传出：**

算法传出的JSON结构如下，需要注意的是，因为格式统一的原因，其中box存储的是数组，但其实result只会有一个，result中存放的是角度信息，类型为float：

```
-bottomNum
-box
    [result]
```

## 二 定位复核算法

**算法传入：**

算法的传入信息包括协议头、图像JSON数据、数据中间分界、base64格式的图像数据与协议尾，如下：

```
{-ALGOHead-}图像JSON数据{-ALGOBound-}图像数据{-ALGOTail-}
```

其中，{-ALGOHead-}为协议头，{-ALGOBound-}为数据中间分界，{-ALGOTail-}为协议尾，图像JSON数据包括如下内容：

```
-imageName
-imageWidth
-imageHeight
-bottomNum
```

**算法传出：**

算法传出的JSON结构如下，需要注意的是，因为格式统一的原因，其中box存储的是数组，但其实result只会有一个，result中存放的是角度信息，类型为float：

```
-bottomNum
-box
    [result]
```

## 三 OCR算法

**算法传入：**

算法的传入信息包括协议头、图像JSON数据、数据中间分界、base64格式的图像数据与协议尾，如下：

```
{-ALGOHead-}图像JSON数据{-ALGOBound-}图像数据{-ALGOTail-}
```

其中，{-ALGOHead-}为协议头，{-ALGOBound-}为数据中间分界，{-ALGOTail-}为协议尾，图像JSON数据包括如下内容：

```
-imageName
-imageWidth
-imageHeight
-bottomNum
```

**算法传出：**

算法传出的JSON结构如下，需要注意的是，因为格式统一的原因，其中box存储的是数组，但其实result只会有一个，result中存放的是角度信息，类型为float：

```
-bottomNum
-box
    [result]
```

## 四 内塞检测算法

