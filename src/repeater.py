'''
当文件里已经有一定量（如2e6条记录）时，使用此函数。
同一个文件不断写入自身，循环复制。
'''
def main():
	with open("../data/data100gb.txt", "r") as file, open("../data/data100gb.txt", "a") as write:
		cnt = 0
		for line in file:
			write.write(line)
			cnt+=1
			if(cnt % 10000 == 0): print(cnt)
			if(cnt>=2e9): break

if __name__ == '__main__':
    main()