

- 代码克隆

git clone --recursive https://github.com/groute/ppopp17-artifact

```gitbash
git clone --recursive https://github.com/groute/ppopp17-artifact
```

- 算例下载



- 运行



- 设置sh文件格式满足需求

```sh
sed -i "s/\r//" *.sh
```

- 使用xshell传输文件

```sh
rz -y
```

- 代码分析笔记
- shell中$的用法：

​		$0：Shell 的命令本身
​		$1 到 $9：表示 Shell 的第几个参数
​		$? ：显示最后命令的执行情况
​		$#：传递到脚本的参数个数
​		$$：脚本运行的当前进程 ID 号
​		$*：以一个单字符串显示所有向脚本传递的参数
​		$!：后台运行的最后一个进程的 ID 号
​		$-：显示 Shell 使用的当前选项

- 文件解压

```sh
bzip2 -d filename
bunzip2 filename
```



- 设置系统代理

```sh
export https_proxy=127.0.0.1:7890
export http_proxy=127.0.0.1:7890
//恢复系统代理
unset https_proxy http_proxy
```

- 检查系统代理

```sh
env|grep -i proxy
```

- 查看正在进行的进程

```sh
ps -a
```

- 运行clash，访问浏览器

```sh
./clash -d ./
curl 
```



- 其他代码信息

```sh
echo "1" >> ./mgbench.exists
echo "1" >> ./nccl.exists
echo "1" >> ./groute.exists
echo "1" >> ./code/mgbench/build/numgpus
```

- 调通程序代码groute
  - 1.检查根目录是否生成五个exist文件和一个patch文件，如没有，需要修改setup.sh将生成相关文件的代码另起一行
  - 2.检查/code/mgbench中是否生成gpunums可执行文件，若未生成，需要主动编译生成相关可执行文件，并将其放置到相关目录中
  - 3.下载算例信息

- 要在Visual Studio中使用NVTX支持编译项目,请使用以下步骤相应地设置项目:
  1. 打开项目属性对话框.
  2. 导航到配置>属性> C/C++>常规.将以下路径添加到其他包含目录:$(NVTOOLSEXT_PATH)\ include
  3. 导航到配置属性>链接器>常规.将以下路径添加到其他库目录:$(NVTOOLSEXT_PATH)\ lib\$(平台)
  4. 导航到配置>属性>链接器>输入.将nvToolsExt32_1.lib或nvToolsExt64_1.lib(根据您的系统规范)添加到其他依赖项.
