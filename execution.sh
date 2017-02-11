#!/bin/sh

echo "totalLearningStep is" 
read LEARNINGSTEP 
echo "totalOutputStep is"
read OUTPUTSTEP
 python RBMprogram/translateToOneliner.py $1input.dat \
&& ./build/rbmlearning $1 $LEARNINGSTEP $OUTPUTSTEP \
&& python3 plot/plotmatrix.py $1answer.dat
python3 plot/plotmatrix.py $1input.dat &
python3 plot/plotconnection.py $1connection.dat &
python3 plot/plotconnection.py $1bias.dat &
python3 plot/plot.py $1differentiation.dat &
wait
display $1input.png &
display $1connection.png &
display $1bias.png &
display $1differentiation.png &
display $1answer.png &
