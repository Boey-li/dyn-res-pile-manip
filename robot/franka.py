import pybullet as p
import pybullet_data as pd
import math
import time
import numpy as np
import pybullet_robots.panda.panda_sim as panda_sim

p.connect(p.GUI) # p.GUI (带渲染); p.DIRECT (不带渲染，适合强化学习训练)
p.configureDebugVisualizer(p.COV_ENABLE_Y_AXIS_UP, 1)
p.setAdditionalSearchPath(pd.getDataPath())

# 设置重力以及仿真器一些参数
timeStep = 1./60.
p.setTimeStep(timeStep)
p.setGravity(0,-9.8,0)

# 加载机器人模型
panda = panda_sim.PandaSim(p, [0,0,0])
while (1):
	panda.step()
	p.stepSimulation()
	time.sleep(timeStep)