# 注意

由于作者使用的是MacOS，所以默认版本适用于Mac系统，其中“实验四win.cpp"为Windows版本，区别仅在于将system("clear"); 改为system("cls");

# 实验思路

## Step 1:图的构建与遍历

图的存储方式或为邻接表，或为邻接矩阵，我在这里觉得邻接表是一维的顺序表，更方便理解一些，于是选择了邻接表。本题中的两个文件非常简洁明了，从"patent.csv"中读取每个结点的信息，从"citation.csv"中读取每条边的信息。邻接表在这里我选择使用vector类型变量存储，并且存储的是结点的地址。在这里我觉得有必要说明一下为什么存地址而不是直接存结点本身：试想一下，由于在后续过程中我们时常需要从顺序表中调取某个结点出来，除了读取它的信息之外可能还要进行修改，那么在我不想改变原vector的结构的情况下，我如果存的是结点，提取出来到一个patent类型的变量p之后，如果对p直接修改，并不会对vector中的结点有任何修改作用，因为提取过程是复制传参；但是我如果存的是指针，那么提取出来一个结点到patent*类型的变量q，我不仅可以获得q中的信息，我对q->（q指向）的内容进行修改的时候，就是直接对结点的内容在修改，vector中存的也是这个结点的指针。除此之外，指针占用空间较小，在提取过程中减少了不必要的复制操作，有效减小空间复杂度。

### 遍历
DFS和BFS遍历相对而言比较简单，需要注意的是与树的遍历区别。DFS中，同样是递归结构，但是在树的遍历中，无论左（右）子术空不空，都会对左（右）子数做一次深度优先遍历，停止递归的条件是函数接收到的指针为空。在图的DFS遍历中，我们对某个结点的后继结点是否执行DFS函数的判断条件是在visited数组中表明该结点未被访问，并没有空不空一说，因为邻接表中存下来的边当然是存在的。还有一点需要注意，因为在DFS遍历中涉及对同一个visited数组的调用和更改，所以我们并不能在函数中去建立这个visited数组，否则递归过程中会建立出一堆不一样的visited，毫无意义。解决方案是在调用函数前建立好一个全部是false的visited数组，设计函数的时候加一个参数visited数组，并且要带上复制符号“&”，保证在都在对同一个visited数组进行更改，调用函数的时候把在函数外建立好的visited数组传进去就好了。相比之下BFS就非常简单了，调用一个队列就好，不过多赘述

## Step 2:Prim算法找最小生成树
这一步非常复杂，原因在于这个算法本身有一定的复杂性，并且在人操作的时候，对于lowcost数组，我们每一次都要找新加的点对这个数组有什么影响，如果对于某个还未加入的顶点来说对小权重有改变，我们不仅仅要记录下来这个最小权重变成了什么，还要记录下来这个最小权重的边是从哪里来的，否则我们在取出这个点的时候不知道连到树的哪个节点上。除此之外，对于已经加入的顶点，它依然会在lowcost数组上，但是这个时候我们并不希望再重复取出该结点，于是我们还需要记录下来我们已经取出了哪些顶点，并且每次在准备取下一个顶点之前，把lowcost的该结点的权重改为10000，用于表示 $\infty$ 。这算相关代码比较复杂，在这里三言两语很难讲清楚，这里只解释其中的lowcost数组的维护方式。首先我这里选用了map类型来存储lowcost的数组，它的键（可以理解为索引）为每个结点的地址，它的值是一个新建的组合类型，其中包含了int类型的权重和patent*类型记录这个权重对应的边的起始点。在每一次取最小值之前需要做两件事，一是对上一次新加的结点的出度进行遍历，看看会不会影响lowcost数组，具体操作就是在lowcost中用每一个的出度的边的终点patent作为键，看这个边的weight是否小于lowcost这个位置之前存的weight，如果小于就要对这个位置的值进行更改。二是把已经加入树中的结点的lowcost数组的weight改成10000，这需要我们在每次取出顶点时不仅把它加入树中，还加入一个顺序表中用于记录这个结点已经加入，之所以要用这个顺序表，是因为每次遍历树是不方便的，但是遍历顺序表很方便。注意：对于map的相关操作这里不加赘述，如果不清楚可以搜索相关帖子，这里说明map类型变量在遍历的时候不能使用for(int i=0;i \< size;i++)的结构，因为map类型变量无法通过“第几号位”来找出，只能用for(patent\* temp:lowcost)这种c++独有的遍历方式来写，其中patent\*可以改为auto，让编译器自己识别类型，temp充当了lowcost当中的每一个元素，取出它的键用temp.first，取出它的值用temp.second。

# 使用说明

## 推荐的使用方式

遇到想不清楚怎么做的部分，看一下我的代码是怎么处理的，然后自己写对应的思路，进行改进

## 不推荐的使用方式

将我的代码丢给各种AI，进行C++与C语言之间的多次翻译，然后对于翻译中丢失的功能再让AI补全

## 禁止的使用方式

直接把我的代码交给老师，或不加来源说明地使用