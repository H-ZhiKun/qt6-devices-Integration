#pragma once
namespace AppFrame
{
	class NonCopyable
	{
	protected:
		NonCopyable()
		{
		}
		~NonCopyable()
		{
		}
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
		NonCopyable(NonCopyable&&) noexcept(true) = default;
		NonCopyable& operator=(NonCopyable&&) noexcept(true) = default;
	};
}
