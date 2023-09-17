import os
import numpy as np
import pyflex
import time


time_step = 500 # 120
# des_dir = 'test_FluidFall'
# os.system('mkdir -p ' + des_dir)

pyflex.init(False)

# scene_params = np.array([])

global_scale = 24

rand_scale = np.random.uniform(0.09, 0.12) * global_scale / 8.0
max_scale = rand_scale
min_scale = rand_scale
blob_r = np.random.uniform(0.7, 1.0)
x = - blob_r * global_scale / 8.0
y = 0.5
z = - blob_r * global_scale / 8.0
inter_space = 1.5 * max_scale
num_x = int(abs(x/1.5) / max_scale + 1) * 2
num_y = 10
num_z = int(abs(z/1.5) / max_scale + 1) * 2
x_off = global_scale * np.random.uniform(-1./24., 1./24.)
z_off = global_scale * np.random.uniform(-1./24., 1./24.)
x += x_off
z += z_off
num_carrots = (num_x * num_z - 1) * 3
add_singular = 0.0
add_sing_x = -1
add_sing_y = -1
add_sing_z = -1
add_noise = 0.0

staticFriction = 1.0
dynamicFriction = 0.9
draw_skin = 1.0
min_dist = 10.0
max_dist = 20.0

scene_params = np.array([max_scale,
                        min_scale,
                        x,
                        y,
                        z,
                        staticFriction,
                        dynamicFriction,
                        draw_skin,
                        num_carrots,
                        min_dist,
                        max_dist,
                        num_x,
                        num_y,
                        num_z,
                        inter_space,
                        add_singular,
                        add_sing_x,
                        add_sing_y,
                        add_sing_z,
                        add_noise,])
# pyflex.set_scene(4, scene_params, 0)
pyflex.set_scene(22, scene_params, 0)

for i in range(time_step):
    # pyflex.step(capture=1, path=os.path.join(des_dir, 'render_%d.tga' % i))
    pyflex.step()

pyflex.clean()
