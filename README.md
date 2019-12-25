# IMSF-boilerplate

A boilerplate for imgui-sfml projects


modifications to make sfml-imgui work with sf::RenderTexture
```cpp
// added in imgui-SFML.h
IMGUI_SFML_API void  Image(const sf::RenderTexture& texture,
	const sf::Color& tintColor = sf::Color::White, 
	const sf::Color& borderColor= sf::Color::Transparent
);

```

```cpp
// added in imgui-SFML.cpp
void Image(const sf::RenderTexture& texture, const sf::Color& tintColor, const sf::Color& borderColor)
{
	sf::FloatRect textureRect(0, 0, texture.getSize().x, texture.getSize().y);

	sf::Vector2f textureSize = static_cast<sf::Vector2f>(texture.getSize());
	ImVec2 uv0(textureRect.left / textureSize.x, (textureRect.top + textureRect.height) / textureSize.y);
	ImVec2 uv1((textureRect.left + textureRect.width) / textureSize.x, textureRect.top / textureSize.y);
	void* _texture = (void *)texture.getTexture().getNativeHandle();
	ImGui::Image(texture.getTexture().getNativeHandle(), textureSize, uv0, uv1, tintColor, borderColor);
}
	
```

modifications in imgui_memory_editor.h
```cpp
#include "imgui.h"
#ifdef _WIN32
	#define sprintf sprintf_s
	#define sscanf sscanf_s
	#undef ImSnprintf
	#define ImSnprintf(out_buf, out_buf_size, ...) _snprintf_s(out_buf, out_buf_size, out_buf_size, __VA_ARGS__)
#endif
```

```cpp
// added argument bool* p_open
void DrawWindow(const char* title, void* mem_data, size_t mem_size, bool* p_open = (bool*)0, size_t base_display_addr = 0x0000);
```