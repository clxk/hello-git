#!/usr/bin/perl
use File::Copy::Recursive qw(fcopy rcopy dircopy fmove rmove dirmove);
#fcopy($orig,$new[,$buf]) or die $!;
#rcopy($orig,$new[,$buf]) or die $!;
#dircopy($orig,$new[,$buf]) or die $!;
#fmove($orig,$new[,$buf]) or die $!;
#rmove($orig,$new[,$buf]) or die $!;
#dirmove($orig,$new[,$buf]) or die $!;
#rcopy_glob("orig/stuff-*", $trg [, $buf]) or die $!;
#rmove_glob("orig/stuff-*", $trg [,$buf]) or die $!;
use File::Spec::Functions;

$argc = @ARGV;
if($argc < 2){ 
    print "usage:\n";
    die "gitcopy inputdir outputdir\n";
}
opendir my $dh,$ARGV[0] or die "Can't open $ARGV[0]: $!\n";
foreach (readdir $dh){
    next if /^[.]/;
    my $sf = catfile($ARGV[0],$_);
    my $df = catfile($ARGV[1],$_);
    #fcopy($sf,$df) if -f $sf;
    #dircopy($sf,$df) if -d $sf;
    rcopy($sf,$df) or die "Can't copy $sf to $df: $!\n";
}
closedir $dh;


#数组操作
#push   &   pop
#shift  &   unshift 
#splice
#reverse
#sort
#each
#scalar     强制指定标量上下文
#<STDIN>在标量上下文中会返回下一行，而在列表上下文中会返回剩下的所有行，直到文件结尾.
#my state   私有变量&持久性私有变量
#while(<STDIN>):每次读取一行 foreach(<STDIN>):一次读取整个文件
#<>:while(<>)   @ARGV
#一般来说，如果数组里的字符串包含换行符，那么只要直接将它们输出来就好了:    print @array;
#要是它们不包含换行符，你通常会想在结尾补上一个:    print "@array\n";
#没有括号时，print是列表操作符，会把其后的列表里的所有东西都输出来
#假如print后面紧跟着左括号，它就是一个函数调用，只会将括号内的东西输出来
#STDIN STDOUT STDERR DATA ARGV ARGVOUT
#open LOG, '>>:encoding(UTF-8)', &logfile_name();
#perl -MEncode -le "print for Encode->encodings(':all')"
#autodie
#select
#say
#如果跟在print后面的第一个参数之后没有逗号，就说明它是一个文件句柄
#胖箭头 => ，对于Perl而言，它只是逗号的另一种写法.
#keys & values & each & exists & delete
#%ENV

#regular expression
#\p{PROPERTY}   \P{PROPERTY}
#metacharacter  -   元字符
#(.) 匹配任意一个字符的通配符，换行符除外
#(* + ?) 量词
#(.)\1 匹配连续出现的两个同样的字符
#(.)(.)\2\1 匹配类似'abba'的字符串
#(.)\111    (.)\g{1}11
#\s \h \v \d \p{Space} = \h|\v = [\h\v] \w = [a-zA-Z0-9_]
#模式匹配修饰符 /a /u /l /i /s /x /aa
#锚位 \A-字符串的绝对开头 \z-字符串的绝对末尾 \Z-行末锚位 \^-行首 \$-行末  /m   \b-单词边界锚位 \B
#绑定操作符=~ : 拿右边的模式来匹配左边的字符串
#捕获(PATTERN) 不捕获(:?PATTERN)  命名捕获(?<LABEL>PATTERN) %+  反向引用\1 \g{1} \g{label} \k<label>
#自动捕获变量 $& $` $'      ${^PREMATCH} ${^MATCH} ${^POSTMATCH}
#通用量词 *={0,} +={1,} ?={0,1}
#查找-m// 替换-s/// 全局替换-/g
#大小写转换 \U \L \u \l \E
#split = split /\s+/,$_
#join $glue,@pieces
#非贪婪量词 *? +? ?? {0,1}?
#my $lines = join '',<FILE> 获取多行文本
#chomp(my $date = `date`)  my $date = localtime
#钻石操作符<>会读取命令行参数指定的那些文件.
#钻石操作符: 它会自动帮你打开许多文件，而且如果没有指定文件，它就会从标准输入读进数据
#但如果$^I中是个字符串，该字符串就会变成备份文件的扩展名
#perl -p -i.bak -w -e 's/Randall/Randal/g' fred*.dat
#-p = while(<>){print;}
#-i.bak = $^I = '.bak'
#-w开启警告
#-e告诉Perl后面跟着的是可供执行的程序代码
#控制结构   unless/if  until/while
#for($_="bedrock";s/(.)//;){print "One character is: $1\n";}
#for和foreach两个关键字实际上是等价的
#循环控制 last next redo
#标签用来命名整块代码，而不是用来标明程序中的某个具体位置
#逻辑操作符 && || defined  //=defined-or  and  or
#ExtUtils::Makemaker    Module::Build
#文件操作符
#-r     文件或目录，对目前（有效）用户或组来说是可读的
#-w     文件或目录，对目前（有效）用户或组来说是可写的
#-x     文件或目录，对目前（有效）用户或组来说是可执行的
#-o     文件或目录，由目前（有效）用户拥有
#-R     文件或目录，对实际的用户或组来说是可读的
#-W     文件或目录，对实际的用户或组来说是可写的
#-X     文件或目录，对实际的用户或组来说是可执行的
#-O     文件或目录，由目前（有效）用户拥有
#-e     文件或目录，是存在的
#-z     文件存在而且没有内容（对目录来说永远为假）
#-s     文件或目录存在而且有内容（返回值是以字节为单位的文件大小）
#-f     是普通文件
#-d     是目录
#-l     是符号链接
#-S     是socket类型的文件
#-p     是命名管道,也就是先入先出(fifo)队列
#-b     是块设备文件（比如某个可挂在的磁盘）
#-c     是字符设备文件（比如某个I/O设备）
#-u     文件或目录设置了setuid位
#-g     文件或目录设置了setgid位
#-k     文件或目录设置了sticky位
#-t     文件句柄是TTY设备(类似系统函数isatty(）的测试，不能对文件名进行此测试
#-T     看起来像文本文件
#-B     看起来像二进制文件
#-M     最后一次被修改后至今的天数
#-A     最后一次被访问后至今的天数
#-C     最后一次文件节点编号被变更后至今的天数
#if(-r $file and -w _){...}
#if(-w -r $file){...}   use 5.010
#stat lstat
#my($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks) = stat($filename);
#my($sec,$min,$hour,$day,$mon,$year,$wday,$yday,$isdst) = localtime $timestamp;
#按位运算操作符 & | ^ << >> ~
#chdir 改变目录 chdir '/etc' or die "cannot chdir to /etc: $!";
#my @all_files = glob '*';      my @pm_files = glob '*.pm';
#my @all_files = glob "*"   =   my @all_files = <*>
#open read close        opendir readdir closedir
#unlink glob "*.o"; 返回值代表成功删除的文件数目
#删除文件的权限跟文件本身的权限位无关，它取决于文件所在目录的权限位.
#批量重命名
#foreach my $file (glob "*.old"){
#    (my $newfile = $file) =~ s/\.old$/.new/;
#    if(-e $newfile){
#        warn "can't rename $file to $newfile: $newfile already exists\n";
#    }elsif(rename $file => $newfile){
#    }else{
#        warn "rename $file to $newfile failed: $!\n";
#    }
#}
#soft link  & symbolic link
#mkdir $name,oct($permissions);
#用index查找子字符串    $where = index($big,$small);    rindex
#my $part = substr($string,$initial_position,$length);
#sub big_money{
#    my $number = sprintf "%.2f", shift @_;
#    1 while $number =~ s/^(-?\d+)(\d\d\d)/$1,$2/;
#    $number =~ s/^(-?)/$1\$/;
#    $number;
#}
#sub by_number{
#    if($a < $b){-1}elsif($a > $b){1}else{0}
#}
#sub by_number {$a <=> $b}
#my @result = sort by_number @some_numbers;
#sub by_string {$a cmp $b}
#my @numbers = sort {$a <=> $b} @some_numbers;
#print "I found Fred in the name!\n" if $name ~~ /Fred/;
#print "I found a key matching 'Fred'" if %names ~~ /Fred/;
#print "I found a key matching 'Fred'" if /Fred/ ~~ %names;
#say "The arrays have the same elements!" if @names1 ~~ @names2;
#say "The result [$result] is one of the input values(@nums)" if @nums ~~ $result;
#匹配类型
#%a ~~ %b               哈希的键是否一致
#%a ~~ @b               %a中的至少一个键在列表@b中
#%a ~~ /Fred/           至少有一个键匹配给定的模式
#'Fred' ~~ %a           是否存在$a{Fred}
#@a ~~ @b               数组是否相同
#@a ~~ /Fred/           @a中至少有一个元素匹配模式
#$name ~~ undef $name   $name没有定义
#$name ~~ /Fred/        模式匹配
#123 ~~ '123.0'         数值和'nmish'类型的字符串是否相等
#'Fred' ~~ 'Fred'       字符串是否相同
#given...when   switch...case
#123 ~~ 456             数值是否相等

#曾经有一个阶段，现在也是，我想过对自己生活的定位。总是觉得自己有些迷茫，有些烦躁,似乎有很多的想法，但是总有懒惰的理由和借口去抹杀
#“有点钱，有点闲”是我理想的生活，呵呵.现在每天都在奔波，工作占了我绝大部分的时间和精力.
#有时候会想到放弃，想要有更多的时间去享受生活。最后还是调整自己的心态.只要是自己做出的选择，就不要后悔,快乐的过每一天吧.
#
#如今的社会，多了势利的人，少了真情对待的人。于是，工作压力大了，竞争加强了，人与人之间变得冷漠了。
#越来越多的人在这社会上成了两面人，表面一套，背后又会来一套。不知道他们这样的做法是否开心，活得自在.
#
#这时，我想到一则笑话：
#一个美国人到大溪地度假，当大溪地人帮他按摩时，老美满心优越感，满脸悲悯地说：
#“如果你们上进点、积极点、勤快点，你们也可以像我们一样到大溪地度假呀！”大溪地人一脸疑惑地说：
#“你辛苦一年，只为了到大溪地过两星期日子，我却是一整年在大溪地享受生活的，我为什么要学你？”
#
#突然很为那些势利之人悲哀，象这样再继续“进化”下去，房子想越换越大。大到无力打扫，再请佣人；
#为了养房贷与佣人，只好拚命工作，有家归不得。那么大房子又有何意义？
#开车时，以车代步，变得四体不勤，日渐臃肿；只好买个脚踏车或跑步机放在卧室踩。
#
#忙的时候，想要休息；休息的时候，想到未来；穷的时候，渴望富有；生活安逸了，担心结果不如预期；
#看明白了，原来不属于自己的，常常心存欲望；握在手里了，又怀念未拥有前的轻松；生命若不是现在，那是何时?
#
#曾一度认为，踏出校门的那天，我就不再年轻！但现在想想，在社会中其实只是个小孩子而已！很多东西，你要被逼着去学的！
#其实心里最不能感受到自己已经开始生活的缘由，就是不停的工作，以及不能时时感受到想要的温暖。
#看来一个人的性格和气质永远也不会实实在在地改变，但还是自己的变数多一些，没有波澜的生活，会让自己失去生活的激情的，
#我还不想在年轻的时候就失去这些。 
#
#这不是我想要的生活！
#我依然不确定自己想要什么样的生活!!!



#system 'ls -l $HOME'       system "ls -l \$HOME"
#system "long_running_command with parameters &"    启动后台进程
#system 'for i in *;do echo ==$i==;cat $i;done';
#system $command_line;
#system '/bin/sh','-c',$command_line;
#%ENV   环境变量
#system - exec      子程序调用 - goto语句
#无论用system还是exec,所执行命令的输出都会送往Perl的标准输出.果使用反引号可以捕获输出结果.
#除了反引号，还可以用更为一般化的引起操作符qx()
#将标准错误合并至标准输出，Unix Shell中写成 2>&1
#请勿使用会读取标准输入的命令，如果不确定，请将标准输入重定向为从/dev/null读取数据.
#通过文件句柄执行外部进程
#open DATE,'date|' or die ...
#open MAIL,'|mail merlyn' or die ...
#用fork进行深入和复杂的工作
#发送以及接受信号   kill 2,4201 or die ...      %SIG
#切片   split       列表切片    数组切片    哈希切片
#出错捕获 eval
#use Try::Tiny;
#try{...}catch{...}finally{...}
#autodie
#use autodie qw(open system :socket);
#my @odd_numbers = grep {$_%2} 1..1000;
#用map把列表元素变形    my @formatted_data = map {&big_money($_)} @data;
#列表工具   List::Util
#use List::Util qw(first sum max maxstr shuffle)


