# 进程 Process

- 概念: 进程是正在执行程序的实例，是资源分配最小的单位，
    - 每个进程都有自己单独的资源区域。
- 守护进程：运行在后台的进程，用于执行特定的系统任务。
- 进程的状态：
    - 运行态，运行态指的就是进程实际占用 CPU 时间片运行时
    - 就绪态，就绪态指的是可运行，但因为其他进程正在运行而处于就绪状态
    - 阻塞态，除非某种外部事件发生，否则进程不能运行
    
    - 进程在运行期间，不断地从一种状态转换到另一种状态，它可以多次处于就绪状态和运行状态，也可以多次处于阻塞状态。
    - 进程表：为实现进程模型，每个进程占用一个进程表项，该进程表项包含了进程状态的重要信息，包括程序计数器、堆栈指针、内存分配状况、所打开文件的状态、帐号和调度信息等。

# 线程 Thread
2.线程
- 线程：cpu调度的最小单位。线程共享进程的资源，多个线程可以共享同一地址空间和其他资源，比如共享全局变量。线程作为进程的一部分，扮演的角色就是怎么利用中央处理器去运行代码。线程关注的是中央处理器的运行，而不是内存等资源的管理。
- 就绪状态：线程对象被创建后，其它线程调用了该对象的start()方法，从而来启动该线程。处于就绪状态的线程，随时可能被CPU调度执行。
- 运行状态: 线程已获得CPU，正在运行。
- 阻塞状态： 阻塞状态是线程因为某种原因放弃CPU使用权，暂时停止运行。

- 为什么多线程？
    - 线程比进程更加轻量级，线程更容易、快捷的创建和销毁。
    - 多CPU系统中，使用线程提高CPU利用率。
    - 并行实体共享同一个地址空间和所有可用数据的能力。

### 任意时刻 CPU 总是运行一个进程，其他进程属于非运行状态。

## 进程间通信
- 管道( pipe )：数据只能**单向流动**，而且只能在具有**亲缘关系**的进程间使用。进程的亲缘关系通常是指父子进程关系。半双工的通信方式（信息可以双向传输，但是不能同时）

- 有名管道 (namedpipe) ： 有名管道也是半双工的通信方式，但是它允许**无亲缘关系**进程间的通信。

- 消息队列( message queue ) ： 消息队列是有消息的链表，存放在内核中并。优点：数据可以有格式，但是任然有大小限制。

- 信号 (sinal ) ：是一种异步的通知机制，用来提醒进程一个事件已经发生。
    - 生命周期: 产生 > 进程中的注册 > 进程中的注销 > 捕获处理
    - 信号的产生: 
        - 硬件事件： 除数为零，无效的存储访问，例如野指针
        - 软件事件： ctrl+c_中断信号_SIGTSTP, ctrl+|_退出信号_SIGQUIT, ctrl+z_停止信号_SIGINT, kill 命令等
    - 信号的阻塞
        - 阻止一个信号的抵达，当一种信号被阻塞时，它仍可以被发送，但是产生的待处理信号不会被接收，直到进程取消对这种信号的阻塞。

- 共享内存(shared memory ) ：共享内存允许两个或更多进程访问同一块内存，就如同 malloc() 函数向不同进程返回了指向同一个物理内存区域的指针。

- 套接字(socket ) ： 套解口也是一种进程间通信机制，与其他通信机制不同的是，它可用于不同及其间的进程通信。

- 信号量(semophore ) ： 信号量是一个计数器，可以用来控制多个进程对共享资源的访问。它常作为一种锁机制，防止某进程正在访问共享资源时，其他进程也访问该资源。


## 线程间通信

- 锁机制：包括互斥锁（mutex)、条件变量、读写锁

    - 互斥锁提供了以排他方式防止数据结构被并发修改的方法。
    - 读写锁允许多个线程同时读共享数据，而对写操作是互斥的。
    - 条件变量可以以原子的方式阻塞进程，直到某个特定条件为真为止。对条件的测试是在互斥锁的保护下进行的。条件变量始终与互斥锁一起使用。
- 信号量机制(Semaphore)：包括无名线程信号量和命名线程信号量

- 信号机制(Signal)：类似进程间的信号处理。

## 锁
- 概念 lock A lock is an abstraction that allows at most one thread to own it at a time. Holding a lock is how one thread tells other threads: “I’m changing this thing, don’t touch it right now.”
- 操作：

- **Acquire/获取** allows a thread to take ownership of a lock. If a thread tries to acquire a lock currently owned by another thread, it blocks until the other thread releases the lock. At that point, it will contend with any other threads that are trying to acquire the lock. At most one thread can own the lock at a time.

- **Release/释放** relinquishes ownership of the lock, allowing another thread to take ownership of it.

- 自旋锁: 只要没有锁上，就不断重试.浪费CPU资源。
```
while (抢锁(lock) == 没抢到) {
}
```
抢锁失败后只要锁的持有状态一直没有改变，那就让出 CPU 给别的线程先执行。

- 互斥锁： 
    -  Mutex is a mutual exclusion object that synchronizes access to a resource。
    多线程共享一个互斥量，然后线程之间去竞争。得到锁的线程可以进入临界区执行代码。
```
while (抢锁(lock) == 没抢到) {
    本线程先去睡了请在这把锁的状态发生改变时再唤醒
}
```
- 死锁 DeadLock
    - Deadlock occurs when concurrent modules are stuck waiting for each other to do something. the signal feature of deadlock is a **cycle of dependencies**.