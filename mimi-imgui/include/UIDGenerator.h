#ifndef _MIMI_UID_GENERATOR_H_
#define _MIMI_UID_GENERATOR_H_ 1

namespace MIMIIMGUI {
	/**
	 *	Unique Identifier generator.
	 *	This is a simple UID generator.
	 *	Each next will be an increment of previosuly.
	 */
	class UIDGenerator {
	  public:
		UIDGenerator(void) noexcept {
			this->nextUID = 0;
			this->nextLUID = 0;
		}

		UIDGenerator(const UIDGenerator &other) = default;

		/**
		 *	@Return next unique id.
		 */
		unsigned int getNextUID(void) noexcept { return this->nextUID++; }

		/**
		 *	@Return next long uniqie id.
		 */
		unsigned long int getNextLUID(void) noexcept { return this->nextLUID++; }

	  private:						/*	Attributes.	*/
		unsigned int nextUID;		/*	*/
		unsigned long int nextLUID; /*	*/
	};

} // namespace MIMIIMGUI

#endif
