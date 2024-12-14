#include "Exit.hpp"
#include "Lerp.hpp"

Exit::Exit(const Core::Ref<Renderer> renderer,const Core::Ref<Window> window, bool& is_running) : m_Renderer(renderer), m_IsRunning(is_running){
    m_Text.LoadFont("resources/fonts/Aileron/Aileron-SemiBold.otf",100);
    
    m_Text.LoadText(renderer,"Thanks for playing",m_BlackColor);

    OnResize(window);

    m_ExitTextKF.Setup(4,[&](float t){
        uint8_t start_value{std::numeric_limits<uint8_t>::min()}, 
                end_value{std::numeric_limits<uint8_t>::max()};
        
        m_BlackColor.r = Stellar::Lerp(start_value,end_value,t);
        m_BlackColor.g = Stellar::Lerp(start_value,end_value,t);
        m_BlackColor.b = Stellar::Lerp(start_value,end_value,t);
        m_Text.LoadText(m_Renderer,"Thanks for playing",m_BlackColor);
    });

}

Exit::~Exit(){

}

void Exit::OnResize(const Core::Ref<Window> window){
    auto[win_w,win_h] = window->GetWindowSize();

    m_Text->SetSize(ObjectSize(win_w * 0.7f, win_h / 2));
    auto size = m_Text->GetSize();
    m_Text->SetPosition({(win_w / 2) - (size.GetWidth() / 2),((win_h / 2) - (size.GetHeight() / 2))});
}

void Exit::OnCreate(){
    m_Renderer->SetRenderDrawColor({255,255,255,255});
}

void Exit::HandleInput([[maybe_unused]] const Core::Ref<EventHandler> event_handler){

}

void Exit::Update(float dt){
   if(m_ExitTextKF.Update(dt)){
    m_IsRunning = false;
   }
}

void Exit::Render(const Core::Ref<Renderer> renderer){
    renderer->Render(m_Text);
}