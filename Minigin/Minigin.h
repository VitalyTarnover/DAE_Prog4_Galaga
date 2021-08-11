#pragma once


//class AudioServiceProvider;
//class AudioService;

struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		
		void Run();
	private:
		void LoadGame() const;
		void Cleanup();
		void BindCommands();
		void UpdateManagers();


		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps. 1000 ms / FPS = ms per frame. 
		const float m_MsPerUpdate = m_MsPerFrame / 1000.0f; 
		SDL_Window* m_Window{};

		int m_WindowWidth;
		int m_WindowHeight;

		//AudioService* m_Audio;
	};
}