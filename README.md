# IMSF-boilerplate

A boilerplate for imgui-sfml projects


modifications in imgui-SFML.h and imgui-SFML.cpp to make sfml-imgui work with sf::RenderTexture
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