#pragma once
class IndexBuffer
{
private:
	unsigned int m_RenderID;
	//Why here is just RenderID is because in actual engine we will probably use several Graphic API
	//In order to have a better way to manage them, we usually have use RenderID to emphasize this feature
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const ;
	void UnBind() const ;

	inline const int getCount() const { return m_Count; }

};

