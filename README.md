# Darts-clone: A clone of Darts (Double-ARray Trie System)


因为 rime  中编译 prism 词典部分 中使用到了当前的项目 darts.h 实现的 Double Array Trie。
为了使用 java 工具进行分析词典，所以将当前项目使用 cmake 配置成可供 java 调用的 JNI。顺便学习一下 相关东西。

其中 JNI  编写部分参考了 [marisa-java](https://github.com/ancel/marisa-java)
<br>另外也有直接使用 java 实现的 darts，参考 [arts-clone-java](https://github.com/hiroshi-manabe/darts-clone-java)

> [!IMPORTANT] 当前只编译了 MAC 上的动态链接库。
