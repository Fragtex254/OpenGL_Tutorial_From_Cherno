#pragma once
class VertexBuffer
{
private:
	unsigned int m_RenderID;
	//Why here is just RenderID is because in actual engine we will probably use several Graphic API
	//In order to have a better way to manage them, we usually have use RenderID to emphasize this feature
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

};

