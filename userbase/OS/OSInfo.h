#pragma once

namespace ubase
{
	enum WIN_OS_VERSION
	{
		WIN_UNKNOWN=0,
		WIN_2000,
		WIN_XP,
		WIN_2003,
		WIN_VISTA,
		WIN_7x32,
		WIN_7x64,
		WIN_8x32,
		WIN_8x64
	};

	class OSInfo
	{
	public:
		OSInfo();
		~OSInfo();
		WIN_OS_VERSION GetOSVersion(); //�������ֻ��׼ȷ��λ��win8���°���win8��ϵͳ�汾
	};
}