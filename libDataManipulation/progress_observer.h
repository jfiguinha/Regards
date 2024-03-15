#pragma once

namespace vole {

	class ProgressObserver
	{
	public:
		/// if false: cancel job
		virtual bool update(int percent) = 0;
	};
}
