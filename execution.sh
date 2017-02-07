#!/bin/sh

echo "totalLearningStep is" 
read LEARNINGSTEP 
echo "totalOutputStep is"
read OUTPUTSTEP \
&& python RBMprogram/translateToOneliner.py $1input.dat \
&& ./build/rbmlerning $1 $LEARNINGSTEP $OUTPUTSTEP \
&& python3 plot/plot.py $1answer.dat
python3 plot/plot.py $1input.dat
python3 plot/plotconnection.py $1connection.dat
