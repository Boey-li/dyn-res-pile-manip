class by_SingleYCB : public Scene
{
public:

	by_SingleYCB(const char* name) : Scene(name) {}

	char* make_path(char* full_path, std::string path) {
		strcpy(full_path, getenv("PYFLEXROOT"));
		strcat(full_path, path.c_str());
		return full_path;
	}

	// void Initialize(py::array_t<float> scene_params, int thread_idx = 0)
	void Initialize(py::array_t<float> scene_params, 
                    py::array_t<float> vertices,
                    py::array_t<int> stretch_edges,
                    py::array_t<int> bend_edges,
                    py::array_t<int> shear_edges,
                    py::array_t<int> faces,
                    int thread_idx = 0)
	{
		auto ptr = (float *) scene_params.request().ptr;

		float x = ptr[0];
		float y = ptr[1];
		float z = ptr[2];
		float scale = ptr[3];
		int type = ptr[4];
		int draw_mesh = ptr[5];
		
		float radius = ptr[6];
		float mass = ptr[7];
		float rigidStiffness = ptr[8]; //1.0
		float dynamicFriction = ptr[9]; //1.0
		float staticFriction = ptr[10]; //0.0
		float viscosity = ptr[11]; //0.0

		float invMass = 1.0f/mass; //0.25
		int group = 0;
		float s = radius*0.5f;

		char path[100];

		if (type == 3)
			make_path(path, "/data/ycb/03_cracker_box.obj");
		else if (type == 4)
			make_path(path, "/data/ycb/04_sugar_box.obj");
		else if (type == 5)
			make_path(path, "/data/ycb/05_tomato_soup_can.obj");
		else if (type == 6)
			make_path(path, "/data/ycb/06_mustard_bottle.obj");
		else if (type == 7)
			make_path(path, "/data/ycb/07_tuna_fish_can.obj");
		else if (type == 8)
			make_path(path, "/data/ycb/08_pudding_box.obj");
		else if (type == 9)
			make_path(path, "/data/ycb/09_gelatin_box.obj");
		else if (type == 10)
			make_path(path, "/data/ycb/10_potted_meat_can.obj");
		else if (type == 12)
			make_path(path, "/data/ycb/12_strawberry.obj");
		else if (type == 13)
			make_path(path, "/data/ycb/13_apple.obj");
		else if (type == 14)
			make_path(path, "/data/ycb/14_lemon.obj");
		else if (type == 15)
			make_path(path, "/data/ycb/15_peach.obj");	
		else if (type == 16)
			make_path(path, "/data/ycb/16_pear.obj");
		else if (type == 17)
			make_path(path, "/data/ycb/17_orange.obj");
		else if (type == 19)
			make_path(path, "/data/ycb/19_pitcher_base.obj");
		else if (type == 21)
			make_path(path, "/data/ycb/21_bleach_cleanser.obj");
		else if (type == 24)
			make_path(path, "/data/ycb/24_bowl.obj");
		else if (type == 25)
			make_path(path, "/data/ycb/25_mug.obj");
		else if (type == 35)
			make_path(path, "/data/ycb/35_power_drill.obj");
		else if (type == 36)
			make_path(path, "/data/ycb/36_wood_block.obj");
		else if (type == 37)
			make_path(path, "/data/ycb/37_scissors.obj");
		
		// void CreateParticleShape(const Mesh* srcMesh, 
		// Vec3 lower, Vec3 scale, float rotation, float spacing, 
		// Vec3 velocity, float invMass, bool rigid, float rigidStiffness, 
		// int phase, bool skin, float jitter=0.005f, Vec3 skinOffset=0.0f, 
		// float skinExpand=0.0f, Vec4 color=Vec4(0.0f), float springStiffness=0.0f, bool texture=false)
		CreateParticleShape(
		        GetFilePathByPlatform(path).c_str(),
				Vec3(x, y, z),
				scale, 0.0f, s, Vec3(0.0f, 0.0f, 0.0f), 
				invMass, true, rigidStiffness, NvFlexMakePhase(group++, 0), true, 0.0f,
				0.0f, 0.0f, Vec4(0.0f), 0.0f, true);

		g_params.radius = radius;
		g_params.fluidRestDistance = radius;
		g_params.numIterations = 4;
		g_params.viscosity = viscosity;
		g_params.dynamicFriction = dynamicFriction;
		g_params.staticFriction = staticFriction;
		g_params.particleCollisionMargin = 1.0f;
		g_params.collisionDistance = g_params.fluidRestDistance*0.5f;
		g_params.vorticityConfinement = 120.0f;
		g_params.cohesion = 0.0025f;
		g_params.drag = 0.06f;
		g_params.lift = 0.f;
		g_params.solidPressure = 1.0f;
		g_params.smoothing = 1.0f;
		g_params.relaxationFactor = 1.0f;

		g_numSubsteps = 2;

		if (draw_mesh) {
			g_drawMesh = true;
			g_drawPoints = false;
			g_drawSprings = false;
		} else {
			g_drawMesh = false;
			g_drawPoints = true;
			g_drawSprings = false;
		};
	}
	
};
