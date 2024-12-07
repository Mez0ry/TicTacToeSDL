#include "Menu.hpp"
#include "SceneManager.hpp"
#include "Playing.hpp"
#include "Exit.hpp"
#include <type_traits>

Menu::Menu(const Core::Ref<Renderer> renderer, const Core::Ref<Window> window,SceneManager& scene_manager) : m_Renderer(renderer), m_SceneManager(scene_manager){
    auto [win_w,win_h] = window->GetWindowSize();

    m_TitlePanelTexture.LoadTexture(renderer,"resources/UI/TitlePanel01.png");
    m_TitlePanelTexture.SetSize(ObjectSize(501,36));
    
    m_BackgroundTexture.LoadTexture(renderer,"resources/UI/MainPanel01.png");
    
    m_BackgroundTexture.SetSize(ObjectSize(win_w,win_h));

    m_RedSwitchPanel.LoadTexture(renderer,"resources/UI/Switch05.png");
    m_GreenSwitchPanel.LoadTexture(renderer,"resources/UI/Switch06.png");

    m_TitleText.LoadFont("resources/fonts/Wombyland/OpenType-TT/WombyLand.ttf",36);
    m_TitleText.LoadText(renderer,"Tic-Tac-Toe",{172,7,62,230});

    m_TitleText->SetSize(ObjectSize(240,36));

    Texture button_texture(renderer,"resources/UI/Button15.png");
    auto buttons_font = Text::StaticLoadFont("resources/fonts/Aileron/Aileron-SemiBold.otf",100);
    
    Button play_button(button_texture,"resources/fonts/Aileron/Aileron-SemiBold.otf","Play",color_white_t),
           exit_button(button_texture,"resources/fonts/Aileron/Aileron-SemiBold.otf","Exit",color_white_t);
    
    play_button.OnClick([&](){
        m_SceneManager.TransitionTo<Menu,Playing>();
    });

    exit_button.OnClick([&](){
        m_SceneManager.TransitionTo<Menu,Exit>();
    });

    m_Buttons.push_back(Core::CreateRef<Button>(play_button));
    m_Buttons.push_back(Core::CreateRef<Button>(exit_button));
    
    OnResize(window);
}

Menu::~Menu(){

}

void Menu::OnResize(const Core::Ref<Window> window) {
    int win_w{},win_h {};

    std::tie(win_w,win_h) = window->GetWindowSize();

    ObjectSize menu_option_size_dst(125,65);

    int y_offset = 0;
    
    for(auto& button : m_Buttons){

        Vec2 pos;
        pos.x = (win_w / 2) - (menu_option_size_dst.GetWidth() / 2);
        pos.y = ((win_h / 2) - (menu_option_size_dst.GetHeight() * 1.2)) + y_offset;
        
        button->SetRect(pos,menu_option_size_dst);
        
        y_offset += (menu_option_size_dst.GetHeight() * 2);
    }

    auto title_size = m_TitleText->GetSize();

    m_TitleText->SetPosition({win_w / 2 - (title_size.GetWidth() / 2), static_cast<int>(win_h * 0.1f)});

    ObjectSize title_panel_size(title_size.GetWidth() * 1.3f,title_size.GetHeight() * 1.5f);
    m_TitlePanelTexture.SetSize(title_panel_size);

    ObjectSize diff = m_TitlePanelTexture.GetSize() - title_size;
    Vec2i pos;
    
    pos.x = (m_TitleText->GetPosition().x - (diff.GetWidth() / 2));
    pos.y = (m_TitleText->GetPosition().y - (diff.GetHeight() / 2));

    m_TitlePanelTexture.SetPosition(pos);
    m_BackgroundTexture.SetSize(ObjectSize(win_w,win_h));
}

void Menu::OnCreate(){
    m_Renderer->SetRenderDrawColor({50,40,90,220});
}

void Menu::HandleInput(const Core::Ref<EventHandler> event_handler){
  for(auto& button : m_Buttons){
    if(button->IsClicked()){
        button->Execute();
    }
    
    if(button->IsHovered()){
        button->ChangeTextColor(color_red_t);
        button->ShareSDLTexture(m_GreenSwitchPanel);
    }else{
        button->ChangeTextColor(color_white_t);
        button->ShareSDLTexture(m_RedSwitchPanel);
    }
  }
}

void Menu::Update(float dt){
    
}

void Menu::Render(const Core::Ref<Renderer> renderer){
    renderer->Render(m_BackgroundTexture);

    for(auto& button : m_Buttons){
        if(button){
            button->Render(renderer);
        }
    }
    
    renderer->Render(m_TitlePanelTexture);
    renderer->Render(m_TitleText); 
}