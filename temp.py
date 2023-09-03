import pyflex
from env.flex_env import FlexEnv, FlexRobotHelper
from utils import load_yaml

# Granular env
# config = load_yaml("config/mpc/config.yaml")
# env = FlexEnv(config)
# env.reset()

pyflex.loadURDF = FlexRobotHelper.loadURDF
pyflex.resetJointState = FlexRobotHelper.resetJointState
pyflex.getRobotShapeStates = FlexRobotHelper.getRobotShapeStates
pyflex.init(False)
while True:
    pyflex.step()
pyflex.clean()
