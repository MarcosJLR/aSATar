import matplotlib.pylab as pl #python -m pip install -U matplotlib
import matplotlib.pyplot as plt
import sys

def representSudoku(name,size,information):
	grid =[]
	for i in range(0,size):
		grid.append([])
		for j in range(0,size):
			grid[i].append(information[i*size+j])
	plt.figure(1,figsize=(size,size))
	tb = plt.table(cellText=grid, loc=(0,0), cellLoc='center',colWidths=[1.0/size for x in range(size)])
	cell = tb.get_celld()
	for i in range(0,size):
		for j in range(0,size):
			cell[(j,i)].set_height(1.0/size)
	ax = plt.gca()
	ax.set_xticks([])
	ax.set_yticks([])
	plt.savefig(name)


def plotTimes(filename,times):
	plt.figure()
	xAxis= range(1,len(times)+1)
	plt.bar(xAxis,times)
	plt.title('Tiempos')
	plt.savefig(filename)


if len(sys.argv)!=2:
	sys.exit()
f = open(sys.argv[1],"r")
lines = f.readlines()
times = []
i=1
tablename=sys.argv[1][0:-4]
for line in lines:
	lineList = line.split()
	if len(lineList)==3:
		representSudoku(tablename+"_sudokus/grid"+str(i)+".png",int(lineList[0])**2,lineList[1])
		times.append(float(lineList[2]))
	i+=1
plotname = tablename+"_plot.png"
plotTimes(plotname,times)

