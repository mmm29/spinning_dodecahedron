#include <fstream>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "engine/engine.h"

#include "engine/platform/sfml/render/sfml_renderer.h"

#include "camera_controller.h"
#include "menu.h"

#include "engine/math/matrix_transform.h"
#include "engine/obj_parser.h"
#include "engine/rigid_body.h"

static sf::Vector2i GetCenterPosition(sf::RenderWindow &window) {
    return sf::Vector2i(static_cast<int>(window.getSize().x / 2), static_cast<int>(window.getSize().y / 2));
}

static void SetMouseInCenter(sf::RenderWindow &window) {
    sf::Mouse::setPosition(GetCenterPosition(window), window);
}

static sf::Vector2i GetMouseMovement(sf::RenderWindow &window) {
    return sf::Mouse::getPosition(window) - GetCenterPosition(window);
}

std::unordered_map<std::string, CameraInfo> *cameras; // TODO: remove it

// Used to read .obj file.
std::string ReadFile(const char *filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::ate);

    if (!file.is_open())
        return std::string();

    std::streamsize file_size = file.tellg();
    file.seekg(0);

    std::string data(file_size, '\0');
    file.read(&data[0], file_size);

    file.close();

    return data;
}

static const char dodecahedron_obj[] =
        "v  -0.57735  -0.57735  0.57735\n"
        "v  0.934172  0.356822  0\n"
        "v  0.934172  -0.356822  0\n"
        "v  -0.934172  0.356822  0\n"
        "v  -0.934172  -0.356822  0\n"
        "v  0  0.934172  0.356822\n"
        "v  0  0.934172  -0.356822\n"
        "v  0.356822  0  -0.934172\n"
        "v  -0.356822  0  -0.934172\n"
        "v  0  -0.934172  -0.356822\n"
        "v  0  -0.934172  0.356822\n"
        "v  0.356822  0  0.934172\n"
        "v  -0.356822  0  0.934172\n"
        "v  0.57735  0.57735  -0.57735\n"
        "v  0.57735  0.57735  0.57735\n"
        "v  -0.57735  0.57735  -0.57735\n"
        "v  -0.57735  0.57735  0.57735\n"
        "v  0.57735  -0.57735  -0.57735\n"
        "v  0.57735  -0.57735  0.57735\n"
        "v  -0.57735  -0.57735  -0.57735\n"
        "f  19  3  2\n"
        "f  12  19  2\n"
        "f  15  12  2\n"
        "f  8  14  2\n"
        "f  18  8  2\n"
        "f  3  18  2\n"
        "f  20  5  4\n"
        "f  9  20  4\n"
        "f  16  9  4\n"
        "f  13  17  4\n"
        "f  1  13  4\n"
        "f  5  1  4\n"
        "f  7  16  4\n"
        "f  6  7  4\n"
        "f  17  6  4\n"
        "f  6  15  2\n"
        "f  7  6  2\n"
        "f  14  7  2\n"
        "f  10  18  3\n"
        "f  11  10  3\n"
        "f  19  11  3\n"
        "f  11  1  5\n"
        "f  10  11  5\n"
        "f  20  10  5\n"
        "f  20  9  8\n"
        "f  10  20  8\n"
        "f  18  10  8\n"
        "f  9  16  7\n"
        "f  8  9  7\n"
        "f  14  8  7\n"
        "f  12  15  6\n"
        "f  13  12  6\n"
        "f  17  13  6\n"
        "f  13  1  11\n"
        "f  12  13  11\n"
        "f  19  12  11";

std::unique_ptr<sf::RenderWindow> CreateWindow(const std::string &name, const sf::VideoMode &video_mode) {
    sf::ContextSettings settings;
    settings.depthBits = 12;
    settings.antialiasingLevel = 1;

    auto window = std::make_unique<sf::RenderWindow>(video_mode, name, sf::Style::Default, settings);

    if (!window->isOpen())
        return nullptr;

    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    window->setMouseCursorVisible(false);

    return window;
}

std::unique_ptr<Engine> CreateEngine(sf::RenderWindow *window) {
    auto engine = std::make_unique<Engine>();

    auto sfml_renderer = std::make_shared<render::SFMLRenderer>();
    sfml_renderer->SetRenderTarget(window);

    std::shared_ptr<render::Renderer> renderer = sfml_renderer;

    ViewPort viewport(static_cast<float>(window->getSize().x),
                      static_cast<float>(window->getSize().y));

    engine->Initialize(viewport, renderer);

    return engine;
}

void InitializeObject(Engine *engine) {
//        std::string obj_mesh_text = ReadFile("obj/dodecahedron.obj");
//        assert(!obj_mesh_text.empty() && "Mesh .obj file not found.");

    std::shared_ptr<Mesh> mesh = ObjParser::Parse(dodecahedron_obj, sizeof(dodecahedron_obj) - 1);
    assert(mesh && "Failed to parse mesh .obj file.");

    mesh->Transform(matrix::Scale(3.f));

    auto obj = std::make_shared<RigidBody>();
    obj->SetMesh(mesh);
    obj->SetColor(Color(0xFF, 0xD3, 0xC9, 0xFF));
    obj->SetWorldPosition(Vector3(10, 10, 10));
    obj->SetRotationVelocity(Vector2(M_PI / 2, M_PI / 2));

    engine->GetWorld()->AddObject(obj);
    engine->GetActiveCamera()->AttachTo(obj);
}

int main() {
    std::unique_ptr<sf::RenderWindow> window = CreateWindow("Spinning dodecahedron", sf::VideoMode(1280, 720));
    if (!window) {
        printf("Unable to create window.\n");
        return 1;
    }

    std::unique_ptr<Engine> engine = CreateEngine(window.get());
    InitializeObject(engine.get());

    auto camera_controller = std::make_shared<CameraController>();
    engine->AttachController(camera_controller);

    sf::Color background_color = sf::Color(0xD7, 0xD7, 0xD7);

    Menu menu(*window);
    menu.Initialize();

    Menu::DrawData menu_data;
    menu_data.window_background_color = &background_color;
    menu_data.engine = engine.get();
    menu_data.cameras["main_camera"] = CameraInfo{
            .camera = engine->GetActiveCamera()
    };
    cameras = &menu_data.cameras; // TODO: remove it

    sf::Clock delta_clock;

    while (window->isOpen()) {
        sf::Time time_elapsed = delta_clock.restart();

        sf::Event event;
        while (window->pollEvent(event)) {
            menu.ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window->close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::F2) {
                    // Menu toggle
                    menu.Toggle();
                    window->setMouseCursorVisible(menu.IsActive());
                    if (!menu.IsActive())
                        SetMouseInCenter(*window);
                } else if (event.key.code == sf::Keyboard::Key::Escape)
                    window->close();
            } else if (!menu.IsActive()) {
                if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mouse_position = sf::Mouse::getPosition(*window);
                    sf::Vector2i center_position = GetCenterPosition(*window);
                    if (mouse_position != center_position)
                        sf::Mouse::setPosition(center_position, *window);

                    sf::Vector2i mouse_movement = mouse_position - center_position;
                    camera_controller->HandleMouseMovement(mouse_movement.x, mouse_movement.y);
                } else if (event.type == sf::Event::MouseEntered) {
                    SetMouseInCenter(*window);
                }
            }
        }

        // Update controllers
        engine->Update(time_elapsed.asSeconds());
        menu.Update(time_elapsed);

        // Drawings
        window->clear(background_color);
        engine->Draw();

        menu.Draw(&menu_data);
        menu.Render();

        window->display();
    }

    menu.Shutdown();

    return 0;
}