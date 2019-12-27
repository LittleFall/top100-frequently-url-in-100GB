def main():
	with open("data100gb.txt", "r") as file, open("data100gb.txt", "a") as write:
		cnt = 0
		for line in file:
			write.write(line)
			cnt+=1
			if(cnt % 10000 == 0): print(cnt)
			if(cnt>=6e7): break

if __name__ == '__main__':
    main()