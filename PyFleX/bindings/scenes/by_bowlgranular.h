
class by_BowlGranular : public Scene
{
public:

	by_BowlGranular(const char* name) : Scene(name) {}

    char* make_path(char* full_path, std::string path) {
		strcpy(full_path, getenv("PYFLEXROOT"));
		strcat(full_path, path.c_str());
		return full_path;
	}

	void Initialize(py::array_t<float> scene_params, 
                    py::array_t<float> vertices,
                    py::array_t<int> stretch_edges,
                    py::array_t<int> bend_edges,
                    py::array_t<int> shear_edges,
                    py::array_t<int> faces,
                    int thread_idx = 0)
	{
		auto ptr = (float *) scene_params.request().ptr;

        float radius = ptr[0];

        float bowl_x = ptr[1];
        float bowl_y = ptr[2];
        float bowl_z = ptr[3];
        float bowl_mass = ptr[13];
        float bowl_scale = ptr[14];

        // add carrots
		int num_x = ptr[4];
		int num_y = ptr[5];
		int num_z = ptr[6];
		float granular_scale = ptr[7];
		float pos_x = ptr[8];
		float pos_y = ptr[9];
		float pos_z = ptr[10];
		float granular_dis = ptr[11];

        int draw_mesh = ptr[12];

		// spoon
		float spoon_scale = ptr[15];
		float spoon_mass = ptr[16];
		float spoon_rotation = ptr[17];

        char bowl_path[100];
        make_path(bowl_path, "/data/bowl.obj");
		char spoon_path[100];
		make_path(spoon_path, "/data/objectverse/spoon.obj");
        
        // add bowl
        float bowl_invMass = 1.0f/bowl_mass; 
        int group = 0;
		
		// void CreateParticleShape(const Mesh* srcMesh, 
		// Vec3 lower, Vec3 scale, float rotation, float spacing, 
		// Vec3 velocity, float invMass, bool rigid, float rigidStiffness, 
		// int phase, bool skin, float jitter=0.005f, Vec3 skinOffset=0.0f, 
		// float skinExpand=0.0f, Vec4 color=Vec4(0.0f), float springStiffness=0.0f, bool texture=false)
        
		// CreateParticleShape(
		//         GetFilePathByPlatform(bowl_path).c_str(),
		// 		Vec3(bowl_x, bowl_y, bowl_z),
		// 		bowl_scale, 0., radius*0.5f, Vec3(0.0f, 0.0f, 0.0f), 
		// 		bowl_invMass, true, 1.0, NvFlexMakePhase(group++, 0), true, 0.0f,
		// 		0.0f, 0.0f, Vec4(204.0f/255.0f, 204.0f/255.0f, 1.0f, 0.0f), 0.0f, false);
        
		// // add spoon
		// float spoon_invMass = 1.0f/spoon_mass; 
		// CreateParticleShape(
		//         GetFilePathByPlatform(spoon_path).c_str(),
		// 		Vec3(bowl_x, bowl_y, bowl_z-1.0f),
		// 		spoon_scale, spoon_rotation, radius*0.5f, Vec3(0.0f, 0.0f, 0.0f), 
		// 		spoon_invMass, true, 1.0, NvFlexMakePhase(group++, 0), true, 0.0f,
		// 		0.0f, 0.0f, Vec4(204.0f/255.0f, 204.0f/255.0f, 1.0f, 0.0f), 0.0f, false);


        float pos_diff = granular_scale + granular_dis;
        // add carrots
		for (int x_idx = 0; x_idx < num_x; x_idx++){
			for (int z_idx = 0; z_idx < num_z; z_idx++) {
				for (int y_idx = 0; y_idx < num_y; y_idx++) {
				int num_planes = Rand(6,12);
				Mesh* m = CreateRandomConvexMesh(num_planes, 5.0f, 10.0f);
				CreateParticleShape(m, Vec3(pos_x + float(x_idx) * pos_diff, pos_y + float(y_idx) * pos_diff, pos_z + float(z_idx) * pos_diff), 0.1f, 0.0f, 
									radius*1.001f, 0.0f, 0.2f, true, 0.8f, NvFlexMakePhase(group++, 0), true, radius*0.1f, 0.0f, 
									0.0f, Vec4(237.0f/255.0f, 145.0f/255.0f, 33.0f/255.0f, 1.0f));	
				}
			}
		}

        g_numSubsteps = 3;

		g_params.numIterations = 6;
		g_params.radius = radius;
		g_params.dynamicFriction = 0.4f;
		g_params.dissipation = 0.001f;
		g_params.sleepThreshold = g_params.radius*0.2f;
		g_params.relaxationFactor = 1.3f;
		g_params.restitution = 0.0f;
		g_params.shapeCollisionMargin = 0.01f;

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