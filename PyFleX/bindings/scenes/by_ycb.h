class by_YCB : public Scene
{
public:

	by_YCB(const char* name) : Scene(name) {}

	char* make_path(char* full_path, std::string path) {
		strcpy(full_path, getenv("PYFLEXROOT"));
		strcat(full_path, path.c_str());
		return full_path;
	}

	void Initialize(py::array_t<float> scene_params, int thread_idx = 0)
	{
		auto ptr = (float *) scene_params.request().ptr;

		float x = ptr[0];
		float y = ptr[1];
		float z = ptr[2];
		float scale = ptr[3];

		float radius = 0.1f;
		int group = 0;

		float s = radius*0.5f;
		float m = 0.25f;

		char path[100];
		// make_path(path, "/data/ycb/pudding_box.ply");
		// make_path(path, "/data/ycb/bowl.ply");
		// make_path(path, "/data/ycb/chips_can.ply");
		// make_path(path, "/data/ycb/pitcher.ply");
		// make_path(path, "/data/ycb/rope_blocks.ply"); // can not work
		// make_path(path, "/data/ycb/spatula.ply"); 
		make_path(path, "/data/ycb/06_mustar_bottle.obj");

		// void CreateParticleShape(const Mesh* srcMesh, 
		// Vec3 lower, Vec3 scale, float rotation, float spacing, 
		// Vec3 velocity, float invMass, bool rigid, float rigidStiffness, 
		// int phase, bool skin, float jitter=0.005f, Vec3 skinOffset=0.0f, 
		// float skinExpand=0.0f, Vec4 color=Vec4(0.0f), float springStiffness=0.0f)
		CreateParticleShape(
		        GetFilePathByPlatform(path).c_str(),
				Vec3(x, y, z),
				scale, 0.0f, s, Vec3(0.0f, 0.0f, 0.0f), 
				m, true, 1.0f, NvFlexMakePhase(group++, 0), true, 0.0f,
				0.0f, 0.0f, Vec4(1.0f, 178.0f/255.0f, 102.0f/255.0f, 1.0f));

		// g_params.radius = radius;
		// g_params.dynamicFriction = 1.0f;
		// g_params.viscosity = 2.0f;
		// g_params.numIterations = 4;
		// g_params.vorticityConfinement = 40.0f;
		// g_params.fluidRestDistance = radius;
		// g_params.solidPressure = 0.f;
		// g_params.relaxationFactor = 0.0f;
		// g_params.cohesion = 0.02f;
		// g_params.collisionDistance = 0.01f;
		// // g_params.restitution = restitution;
		// g_params.gravity[1] = -9.8f;

		g_params.radius = radius;
		g_params.fluidRestDistance = radius;
		g_params.numIterations = 4;
		g_params.viscosity = 1.0f;
		g_params.dynamicFriction = 1.0f;
		g_params.staticFriction = 0.0f;
		g_params.particleCollisionMargin = 0.0f;
		g_params.collisionDistance = g_params.fluidRestDistance*0.5f;
		g_params.vorticityConfinement = 120.0f;
		g_params.cohesion = 0.0025f;
		g_params.drag = 0.06f;
		g_params.lift = 0.f;
		g_params.solidPressure = 0.0f;
		g_params.smoothing = 1.0f;
		g_params.relaxationFactor = 1.0f;

		g_numSubsteps = 2;

		g_drawMesh = true;
		g_drawPoints = false;
		g_drawSprings = false;
	}
	
};
