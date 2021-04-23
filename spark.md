
- Spark section-0 基础 (3)
- Spark section-1 Spark运行细节 (13)
- Spark section-2 Spark 与 Hadoop/MapReduce 比较 (7)
- Spark section-3 RDD (4)
- Spark section-4 RDD操作 (13)
- Spark section-5 大数据问题 (7)
- Spark section-6 机器学习算法 (4)
- Spark section-7 Hive (2)

- Spark section-0 基础(3)
1. spark的有几种部署模式，每种模式特点？

    - 本地模式
        - Spark不一定非要跑在hadoop集群，可以在本地，起多个线程的方式来指定。方便调试，本地模式分三类
        - local：只启动一个executor
        - local[k]: 启动k个executor
        - local：启动跟cpu数目相同的 executor

    - standalone模式
        - 分布式部署集群，自带完整的服务，资源管理和任务监控是Spark自己监控，这个模式也是其他模式的基础
    - Spark on yarn模式
        - 分布式部署集群，资源和任务监控交给yarn管理
        - Spark粗粒度资源分配方式，包含cluster和client运行模式
        - cluster 适合生产，driver运行在集群子节点，具有容错功能
        - client 适合调试，dirver运行在客户端
        - Spark On Mesos模式
2. Spark技术栈有哪些组件，每个组件都有什么功能，适合什么应用场景？

    - Spark core
        - 是其它组件的基础，spark的内核
        - 主要包含：有向循环图、RDD、Lingage、Cache、broadcast等
    - SparkStreaming
        - 是一个对实时数据流进行高通量、容错处理的流式处理系统
        - 将流式计算分解成一系列短小的批处理作业
    - Spark sql：
        - 能够统一处理关系表和RDD，使得开发人员可以轻松地使用SQL命令进行外部查询
    - MLBase
        - 是Spark生态圈的一部分专注于机器学习，让机器学习的门槛更低
        - MLBase分为四部分：MLlib、MLI、ML Optimizer和MLRuntime。
    - GraphX
        - 是Spark中用于图和图并行计算
3. spark有哪些组件

    - master：管理集群和节点，不参与计算。
    - worker：计算节点，进程本身不参与计算，和master汇报。
    - Driver：运行程序的main方法，创建spark context对象。
    - spark context：控制整个application的生命周期，包括dagsheduler和task scheduler等组件。
    - client：用户提交程序的入口。
    - Spark section-1 Spark运行细节(13)
1. spark工作机制
    - 用户在client端提交作业后，会由Driver运行main方法并创建spark context上下文。
    - 执行add算子，形成dag图输入dagscheduler
    - 按照add之间的依赖关系划分stage输入task scheduler
    - task scheduler会将stage划分为taskset分发到各个节点的executor中执行

2. Spark应用程序的执行过程

    - 构建Spark Application的运行环境（启动SparkContext）
    - SparkContext向资源管理器（可以是Standalone、Mesos或YARN）注册并申请运行Executor资源；
    - 资源管理器分配Executor资源，Executor运行情况将随着心跳发送到资源管理器上；
    - SparkContext构建成DAG图，将DAG图分解成Stage，并把Taskset发送给Task Scheduler
    - Executor向SparkContext申请Task，Task Scheduler将Task发放给Executor运行，SparkContext将应用程序代码发放给Executor。
    - Task在Executor上运行，运行完毕释放所有资源。
3. driver的功能是什么？

    - 一个Spark作业运行时包括一个Driver进程，也是作业的主进程，具有main函数，并且有SparkContext的实例，是程序的人口点；
    
    功能：
    - 向集群申请资源
    - 负责了作业的调度和解析
    - 生成Stage并调度Task到Executor上（包括DAGScheduler，TaskScheduler）
4. Spark中Work的主要工作是什么？

    - 管理当前节点内存，CPU的使用状况，接收master分配过来的资源指令，通过ExecutorRunner启动程序分配任务
    - worker就类似于包工头，管理分配新进程，做计算的服务，相当于process服务
    - worker不会运行代码，具体运行的是Executor是可以运行具体appliaction写的业务逻辑代码

5. task有几种类型？2种

    - resultTask类型，最后一个task
    - shuffleMapTask类型，除了最后一个task都是

6. 什么是shuffle，以及为什么需要shuffle？
    - shuffle中文翻译为洗牌，需要shuffle的原因是：某种具有共同特征的数据汇聚到一个计算节点上进行计算

7. Spark master HA 主从切换过程不会影响集群已有的作业运行，为什么？

    - 因为程序在运行之前，已经申请过资源了，driver和Executors通讯，不需要和master进行通讯的。
8. Spark并行度怎么设置比较合适

    - spark并行度，每个core承载2~4个partition（并行度） 并行读和数据规模无关，只和内存和cpu有关

9. Spaek程序执行，有时候默认为什么会产生很多task，怎么修改默认task执行个数？

    - 有很多小文件的时候，有多少个输入block就会有多少个task启动
    - spark中有partition的概念，每个partition都会对应一个task，task越多，在处理大规模数据的时候，就会越有效率
10. Spark中数据的位置是被谁管理的？

    - 每个数据分片都对应具体物理位置，数据的位置是被blockManager管理
11. 为什么要进行序列化

    - 减少存储空间，高效存储和传输数据，缺点：使用时需要反序列化，非常消耗CPU
12. Spark如何处理不能被序列化的对象？

    - 封装成object
13. Spark提交你的jar包时所用的命令是什么？

    - spark-submit
Spark section-2 Spark 与 Hadoop/MapReduce 比较(7)
1. Mapreduce和Spark的相同和区别

    - 两者都是用mr模型来进行并行计算
    - hadoop的一个作业：job
    - job分为map task和reduce task，每个task都是在自己的进程中运行的
    - 当task结束时，进程也会结束
    - spark用户提交的任务：application
    - 一个application对应一个sparkcontext，app中存在多个job
    - 每触发一次action操作就会产生一个job
    - 这些job可以并行或串行执行
    - 每个job中有多个stage，stage是shuffle过程中DAGSchaduler通过RDD之间的依赖关系划分job而来的
每个stage里面有多个task，组成taskset有TaskSchaduler分发到各个executor中执行
executor的生命周期是和app一样的，即使没有job运行也是存在的，所以task可以快速启动读取内存进行计算。
hadoop的job只有map和reduce操作，表达能力比较欠缺
在mr过程中会重复的读写hdfs，造成大量的io操作，多个job需要自己管理关系。
spark的迭代计算都是在内存中进行的
API中提供了大量的RDD操作如join，groupby等
通过DAG图可以实现良好的容错
2. 简答说一下hadoop的mapreduce编程模型

    - 首先map task会从本地文件系统读取数据，转换成key-value形式的键值对集合，使用的是hadoop内置的数据类型（longwritable、text）
    将键值对集合输入mapper进行业务处理过程，将其转换成需要的key-value在输出
    之后会进行一个partition分区操作，默认使用的是hashpartitioner，自定义分区：重写getpartition方法
    之后会对key进行进行sort排序，grouping分组操作将相同key的value合并分组输出
    之后进行一个combiner归约操作，其实就是一个本地段的reduce预处理，以减小后面shufle和reducer的工作量
    reduce task会通过网络将各个数据收集进行reduce处理
    最后将数据保存或者显示，结束整个job

3. 简单说一下hadoop和spark的shuffle相同和差异？

high-level 角度：
两者并没有大的差别 都是将 mapper（Spark: ShuffleMapTask）的输出进行 partition，不同的 partition 送到不同的 reducer（Spark 里 reducer 可能是下一个 stage 里的 ShuffleMapTask，也可能是 ResultTask）
Reducer 以内存作缓冲区，边 shuffle 边 aggregate 数据，等到数据 aggregate 好以后进行 reduce()。
low-level 角度：
Hadoop MapReduce 是 sort-based，进入 combine() 和 reduce() 的 records 必须先 sort。
好处：combine/reduce() 可以处理大规模的数据
因为其输入数据可以通过外排得到
mapper 对每段数据先做排序
reducer 的 shuffle 对排好序的每段数据做归并
Spark 默认选择的是 hash-based，通常使用 HashMap 来对 shuffle 来的数据进行 aggregate，不提前排序
如果用户需要经过排序的数据：sortByKey()
实现角度：
Hadoop MapReduce 将处理流程划分出明显的几个阶段：map(), spilt, merge, shuffle, sort, reduce()
Spark 没有这样功能明确的阶段，只有不同的 stage 和一系列的 transformation()，spill, merge, aggregate 等操作需要蕴含在 transformation() 中
4. 简单说一下hadoop和spark的shuffle过程

hadoop：map端保存分片数据，通过网络收集到reduce端
spark：spark的shuffle是在DAGSchedular划分Stage的时候产生的，TaskSchedule要分发Stage到各个worker的executor，减少shuffle可以提高性能
5. partition和block的关联

hdfs中的block是分布式存储的最小单元，等分，可设置冗余，这样设计有一部分磁盘空间的浪费，但是整齐的block大小，便于快速找到、读取对应的内容
Spark中的partition是RDD的最小单元，RDD是由分布在各个节点上的partition组成的。
partition是指的spark在计算过程中，生成的数据在计算空间内最小单元
同一份数据（RDD）的partion大小不一，数量不定，是根据application里的算子和最初读入的数据分块数量决定
block位于存储空间；partion位于计算空间，block的大小是固定的、partion大小是不固定的，是从2个不同的角度去看数据。
6. Spark为什么比mapreduce快？

基于内存计算，减少低效的磁盘交互
高效的调度算法，基于DAG
容错机制Linage
7. Mapreduce操作的mapper和reducer阶段相当于spark中的哪几个算子？

相当于spark中的map算子和reduceByKey算子，区别：MR会自动进行排序的，spark要看具体partitioner
Spark section-3 RDD(4)
1. RDD机制

分布式弹性数据集，简单的理解成一种数据结构，是spark框架上的通用货币
所有算子都是基于rdd来执行的
rdd执行过程中会形成dag图，然后形成lineage保证容错性等
从物理的角度来看rdd存储的是block和node之间的映射
2. RDD的弹性表现在哪几点？

自动的进行内存和磁盘的存储切换；
基于Lingage的高效容错；
task如果失败会自动进行特定次数的重试；
stage如果失败会自动进行特定次数的重试，而且只会计算失败的分片；
checkpoint和persist，数据计算之后持久化缓存
数据调度弹性，DAG TASK调度和资源无关
数据分片的高度弹性，a.分片很多碎片可以合并成大的，b.par
3. RDD有哪些缺陷？

不支持细粒度的写和更新操作（如网络爬虫）
spark写数据是粗粒度的，所谓粗粒度，就是批量写入数据 （批量写）
但是读数据是细粒度的也就是说可以一条条的读 （一条条读）
不支持增量迭代计算，Flink支持
4. 什么是RDD宽依赖和窄依赖？

RDD和它依赖的parent RDD(s)的关系有两种不同的类型
窄依赖：每一个parent RDD的Partition最多被子RDD的一个Partition使用 （一父一子）
宽依赖：多个子RDD的Partition会依赖同一个parent RDD的Partition （一父多子）
Spark section-4 RDD操作(13)
**1. cache和pesist的区别 **

cache和persist都是用于缓存RDD，避免重复计算
.cache() == .persist(MEMORY_ONLY)
2. cache后面能不能接其他算子,它是不是action操作？

可以接其他算子，但是接了算子之后，起不到缓存应有的效果，因为会重新触发cache
cache不是action操作
3. 什么场景下要进行persist操作？
以下场景会使用persist

某个步骤计算非常耗时或计算链条非常长，需要进行persist持久化
shuffle之后为什么要persist，shuffle要进性网络传输，风险很大，数据丢失重来，恢复代价很大
shuffle之前进行persist，框架默认将数据持久化到磁盘，这个是框架自动做的。
4. rdd有几种操作类型？三种！！

transformation，rdd由一种转为另一种rdd
action
cronroller，控制算子(cache/persist) 对性能和效率的有很好的支持
5. reduceByKey是不是action？

不是，很多人都会以为是action，reduce rdd是action
6. collect功能是什么，其底层是怎么实现的？

driver通过collect把集群中各个节点的内容收集过来汇总成结果
collect返回结果是Array类型的，合并后Array中只有一个元素，是tuple类型（KV类型的）的。
7. map与flatMap的区别

map：对RDD每个元素转换，文件中的每一行数据返回一个数组对象
flatMap：对RDD每个元素转换，然后再扁平化，将所有的对象合并为一个对象，会抛弃值为null的值
8. 列举你常用的action？
collect，reduce,take,count,saveAsTextFile等

9. union操作是产生宽依赖还是窄依赖？

窄依赖
10. Spark累加器有哪些特点？

全局的，只增不减，记录全局集群的唯一状态
在exe中修改它，在driver读取
executor级别共享的，广播变量是task级别的共享
两个application不可以共享累加器，但是同一个app不同的job可以共享
11. spark hashParitioner的弊端

分区原理：对于给定的key，计算其hashCode
弊端是数据不均匀，容易导致数据倾斜
12. RangePartitioner分区的原理

尽量保证每个分区中数据量的均匀，而且分区与分区之间是有序的，也就是说一个分区中的元素肯定都是比另一个分区内的元素小或者大
分区内的元素是不能保证顺序的
简单的说就是将一定范围内的数映射到某一个分区内
13. Spark中的HashShufle的有哪些不足？

shuffle产生海量的小文件在磁盘上，此时会产生大量耗时的、低效的IO操作；
容易导致内存不够用，由于内存需要保存海量的文件操作句柄和临时缓存信息
容易出现数据倾斜，导致OOM
Spark section-5 大数据问题(7)
1. 如何使用Spark解决TopN问题？（互联网公司常面）
https://blog.csdn.net/oopsoom/article/details/25815443

2. 如何使用Spark解决分组排序问题？（互联网公司常面）
https://blog.csdn.net/huitoukest/article/details/51273143

3. 给定a、b两个文件，各存放50亿个url，每个url各占64字节，内存限制是4G，让你找出a、b文件共同的url?

方案1：可以估计每个文件安的大小为5G×64=320G，远远大于内存限制的4G。所以不可能将其完全加载到内存中处理。考虑采取分而治之的方法。
遍历文件a，对每个url求取hash(url)%1000，然后根据所取得的值将url分别存储到1000个小文件(记为a0,a1,…,a999)中。这样每个小文件的大约为300M。
遍历文件b，采取和a相同的方式将url分别存储到1000小文件(记为b0,b1,…,b999)。这样处理后，所有可能相同的url都在对应的小文件(a0vsb0,a1vsb1,…,a999vsb999)中，不对应的小文件不可能有相同的url。然后我们只要求出1000对小文件中相同的url即可。
求每对小文件中相同的url时，可以把其中一个小文件的url存储到hash_set中。然后遍历另一个小文件的每个url，看其是否在刚才构建的hash_set中，如果是，那么就是共同的url，存到文件里面就可以了。
方案2：如果允许有一定的错误率，可以使用Bloomfilter，4G内存大概可以表示340亿bit。将其中一个文件中的url使用Bloomfilter映射为这340亿bit，然后挨个读取另外一个文件的url，检查是否与Bloomfilter，如果是，那么该url应该是共同的url(注意会有一定的错误率)。
4. 有一个1G大小的一个文件，里面每一行是一个词，词的大小不超过16字节，内存限制大小是1M，要求返回频数最高的100个词。

Step1：顺序读文件中，对于每个词x，取hash(x)%5000，然后按照该值存到5000个小文件(记为f0,f1,...,f4999)中，这样每个文件大概是200k左右，如果其中的有的文件超过了1M大小，还可以按照类似的方法继续往下分，直到分解得到的小文件的大小都不超过1M;
Step2：对每个小文件，统计每个文件中出现的词以及相应的频率(可以采用trie树/hash_map等)，并取出出现频率最大的100个词(可以用含100个结点的最小堆)，并把100词及相应的频率存入文件，这样又得到了5000个文件;
Step3：把这5000个文件进行归并(类似与归并排序);
5. 现有海量日志数据保存在一个超级大的文件中，该文件无法直接读入内存，要求从中提取某天出访问百度次数最多的那个IP。
分而治之+Hash
1)IP地址最多有2^32=4G种取值情况，所以不能完全加载到内存中处理;
2)可以考虑采用“分而治之”的思想，按照IP地址的Hash(IP)%1024值，把海量IP日志分别存储到1024个小文件中。这样，每个小文件最多包含4MB个IP地址;
3)对于每一个小文件，可以构建一个IP为key，出现次数为value的Hashmap，同时记录当前出现次数最多的那个IP地址;
4)可以得到1024个小文件中的出现次数最多的IP，再依据常规的排序算法得到总体上出现次数最多的IP;

6. 在2.5亿个整数中找出不重复的整数，注，内存不足以容纳这2.5亿个整数。

方案1：采用2-Bitmap(每个数分配2bit，00表示不存在，01表示出现一次，10表示多次，11无意义)进行，共需内存2^32*2bit=1GB内存，还可以接受。然后扫描这2.5亿个整数，查看Bitmap中相对应位，如果是00变01，01变10，10保持不变。所描完事后，查看bitmap，把对应位是01的整数输出即可。
方案2：也可采用与第1题类似的方法，进行划分小文件的方法。然后在小文件中找出不重复的整数，并排序。然后再进行归并，注意去除重复的元素。
7. 腾讯面试题：给40亿个不重复的unsignedint的整数，没排过序的，然后再给一个数，如何快速判断这个数是否在那40亿个数当中?

申请512M的内存，一个bit位代表一个unsignedint值。读入40亿个数，设置相应的bit位，读入要查询的数，查看相应bit位是否为1，为1表示存在，为0表示不存在。
Spark section-6 机器学习算法(4)
**1. mllib支持的算法？ **

分类、聚类、回归、协同过滤
2. kmeans算法原理

随机初始化中心点范围，计算各个类别的平均值得到新的中心点。
重新计算各个点到中心值的距离划分，再次计算平均值得到新的中心点，直至各个类别数据平均值无变化。
3. 朴素贝叶斯分类算法原理
对于待分类的数据和分类项，根据待分类数据的各个特征属性，出现在各个分类项中的概率判断该数据是属于哪个类别的。

4. 关联规则挖掘算法apriori原理

一个频繁项集的子集也是频繁项集，针对数据得出每个产品的支持数列表，过滤支持数小于预设值的项，对剩下的项进行全排列，重新计算支持数，再次过滤，重复至全排列结束，可得到频繁项和对应的支持数。
Spark section-7 Hive(2)
**1. Hive中存放是什么？ **

表（数据+元数据） 存的是和hdfs的映射关系，hive是逻辑上的数据仓库，实际操作的都是hdfs上的文件，HQL就是用sql语法来写的mr程序。
**2. Hive与关系型数据库的关系？ **

没有关系，hive是数据仓库，不能和数据库一样进行实时的CURD操作。
是一次写入多次读取的操作，可以看成是ETL工具。
Spark服务端口

8080 spark集群web ui端口
4040 sparkjob监控端口
18080 jobhistory端口
Spark Job 默认的调度模式 - FIFO
RDD 特点 - 可分区/可序列化/可持久化
Broadcast - 任何函数调用/是只读的/存储在各个节点
Accumulator - 支持加法/支持数值类型/可并行
Task 数量由 Partition 决定
Task 运行在 Workder node 中 Executor 上的工作单元
master 和 worker 通过 Akka 方式进行通信的
默认的存储级别 - MEMORY_ONLY
hive 的元数据存储在 derby 和 MySQL 中有什么区别 - 多会话
DataFrame 和 RDD 最大的区别 - 多了 schema