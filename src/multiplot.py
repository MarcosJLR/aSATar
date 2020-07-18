import matplotlib.pylab as pl #python -m pip install -U matplotlib
import matplotlib.pyplot as plt
import numpy as np
import sys

def plotTimes(filename,aTimes,bTimes):
	n=len(aTimes)
	plot = plt.subplot(111)
	xAxis= np.arange(n)
	plot.bar(xAxis-0.2,aTimes,0.4,color='orange',align='center',label='aSatar')
	plot.bar(xAxis+0.2,bTimes,0.4,color='teal',align='center',label='zchaff')
	plot.legend(loc="upper left")
	plot.set_title('Tiempos')
	plot.figure.savefig(filename)


if len(sys.argv)!=2:
	sys.exit()
fz = open(sys.argv[1]+"zchaff.txt","r")
linesZ = fz.readlines()
timesZ = []
i=1
for line in linesZ:
	lineList = line.split()
	timesZ.append(float(lineList[-1]))
	i+=1
fa = open(sys.argv[1]+"aSatar.txt","r")
linesA = fa.readlines()
timesA = []
i=1
for line in linesA:
	lineList = line.split()
	timesA.append(float(lineList[-1]))
	i+=1
plotname = sys.argv[1]+"times_plot.png"
plotTimes(plotname,timesA,timesZ)

