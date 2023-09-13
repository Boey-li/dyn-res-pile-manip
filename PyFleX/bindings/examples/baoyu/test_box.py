import os
import numpy as np
import pyflex
import time

def rand_float(lo, hi):
    return np.random.rand() * (hi - lo) + lo

time_step = 1000 # 120
# des_dir = 'test_FluidFall'
# os.system('mkdir -p ' + des_dir)

pyflex.init(False)

n_instance = 3
dynamic_friction = 0.1
gravity = -9.8
restitution = 0.1
low_bound = 0.09

scene_params = np.zeros(n_instance * 3 + 3)
scene_params[0] = n_instance
scene_params[1] = gravity

for i in range(n_instance):
    x = rand_float(0., 0.1)
    y = rand_float(low_bound, low_bound + 0.01)
    z = rand_float(0., 0.1)
    
    scene_params[3*i+2] = x
    scene_params[3*i+3] = y
    scene_params[3*i+4] = z
    
    low_bound += 0.21

pyflex.set_scene(3, scene_params, 0)

for j in range(time_step):
    # pyflex.step(capture=1, path=os.path.join(des_dir, 'render_%d.tga' % i))
    pyflex.step()

pyflex.clean()
