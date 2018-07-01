package effective_Go

import (
	"fmt"
	"runtime"
	"sync"
	"testing"
)

func deferCall() {
	defer func() { fmt.Println("打印前") }()
	defer func() { fmt.Println("打印中") }()
	defer func() { fmt.Println("打印后") }()

	panic("触发异常")
}

//defer 是后进先出。
//panic 需要等defer 结束后才会向上传递。 出现panic恐慌时候，会先按照defer的后入先出的顺序执行，最后才会执行panic。
func TestDeferCall(t *testing.T) {
	deferCall()
}

//////////////////////////////////////////////////////////////////////////////////////////
type student struct {
	Name string
	Age  int
}

func fmtMap(m map[string]*student) string {
	var str string
	for _, v := range m {
		str += fmt.Sprintf("%v;", *v)
	}
	return str
}

//range使用的差异：
//m1的访问：m[stu.Name]=&stu实际上一致指向同一个指针， 最终该指针的值为遍历的最后一个struct的值拷贝。
//m2的访问：将stu赋值给了s，每个循环都有一个新的s。
func TestTraverse(t *testing.T) {
	stus := []student{
		{Name: "zhou", Age: 24},
		{Name: "li", Age: 23},
		{Name: "wang", Age: 22},
	}

	m1 := make(map[string]*student)
	for _, stu := range stus {
		m1[stu.Name] = &stu
	}
	fmt.Println("m1:", fmtMap(m1))

	m2 := make(map[string]*student)
	for _, stu := range stus {
		s := stu
		m2[stu.Name] = &s
	}
	fmt.Println("m2:", fmtMap(m2))
}

//////////////////////////////////////////////////////////////////////////////////////////

//A:均为输出10或者头几次可能出现9、8等，B:从0~9输出(顺序不定)。
//
//第一个go func中i是外部for的一个变量，地址不变化。遍历完成后，最终i=10。
//第二个go func中i是函数参数，与外部for中的i完全是两个变量。 尾部(i)将发生值拷贝，go func内部指向值拷贝地址。
func Testoncurrent(t *testing.T) {
	runtime.GOMAXPROCS(1)
	wg := sync.WaitGroup{}
	wg.Add(20)

	for i := 0; i < 10; i++ {
		go func() {
			fmt.Println("A: ", i)
			wg.Done()
		}()
	}

	for i := 0; i < 10; i++ {
		go func(i int) {
			fmt.Println("B: ", i)
			wg.Done()
		}(i)
	}

	wg.Wait()
}

//////////////////////////////////////////////////////////////////////////////////////////

type People struct{}

func (p *People) ShowA() {
	fmt.Println("showA")
	p.ShowB()
}

func (p *People) ShowB() {
	fmt.Println("showB")

}

type Teacher struct {
	People
}

func (t *Teacher) ShowB() {
	fmt.Println("teacher showB")
}

//这是Golang的组合模式，可以实现OOP的继承。
//被组合的类型People所包含的方法虽然升级成了外部类型Teacher这个组合类型的方法（一定要是匿名字段），但它们的方法(ShowA())调用时接受者并没有发生变化。
//此时People类型并不知道自己会被什么类型组合，当然也就无法调用方法时去使用未知的组合者Teacher类型的功能。
func TestInherit(t *testing.T) {
	te := Teacher{}
	te.ShowA()
}

//select会随机选择一个可用通用做收发操作。 所以代码是有肯触发异常，也有可能不会。
//
//select 中只要有一个case能return，则立刻执行。
//当如果同一时间有多个case均能return则伪随机方式抽取任意一个执行。
//如果没有一个case能return则可以执行”default”块。
func TestChan(t *testing.T) {
	runtime.GOMAXPROCS(1)
	int_chan := make(chan int, 1)
	string_chan := make(chan string, 1)

	int_chan <- 1
	string_chan <- "hello"
	select {
	case value := <-int_chan:
		fmt.Println(value)

	case value := <-string_chan:
		panic(value)
	}
}

func calc(index string, a, b int) int {
	ret := a + b
	fmt.Println(index, a, b, ret)
	return ret
}

//defer执行顺序和值传递 index:1肯定是最后执行的，但是index:1的第三个参数是一个函数，所以最先被调用calc("10",1,2)==>10,1,2,3
//执行index:2时,与之前一样，需要先调用calc("20",0,2)==>20,0,2,2
//执行到b=1时候开始调用，index:2==>calc("2",0,2)==>2,0,2,2
//最后执行index:1==>calc("1",1,3)==>1,1,3,4
func TestDeferSeq(t *testing.T) {
	a := 1
	b := 2
	defer calc("1", a, calc("10", a, b))

	a = 0
	defer calc("2", a, calc("20", a, b))

	b = 1
}

//[0 0 0 0 0 1 2 3]
//[1 2 3]
func TestString(t *testing.T) {
	s1 := make([]int, 5)
	s1 = append(s1, 1, 2, 3)
	fmt.Println(s1)

	s2 := make([]int, 0)
	s2 = append(s2, 1, 2, 3)
	fmt.Println(s2)
}

//////////////////////////////////////////
type People1 interface {
	Show()
}

type Student1 struct{}

func (stu *Student1) Show() {
}

func live() People1 {
	var stu *Student1
	return stu
}

func TestNil(t *testing.T) {
	if live() == nil {
		fmt.Println("AAAAAAA")
	} else {
		fmt.Println("BBBBBBB")
	}
}
