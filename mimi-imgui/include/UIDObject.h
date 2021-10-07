
#ifndef _MIMI_UID_OBJECT_H_
#define _MIMI_UID_OBJECT_H_ 1
#include "UIDGenerator.h"

namespace MIMIIMGUI {
	/**
	 * Responsible for containing
	 * the UID of object.
	 */
	class UIDObject {
	  public:
		UIDObject(void) : UIDObject(UIDObject::generator) {}
		UIDObject(UIDGenerator &uidGenerator) noexcept { this->setUID(uidGenerator.getNextUID()); }
		UIDObject(UIDObject &&other) noexcept = default;
		UIDObject(const UIDObject &other) noexcept = default;
		/**
		 * @brief Set Unique ID for the object.
		 *
		 * @param uid
		 */
		void setUID(unsigned int uid) noexcept { this->uid = uid; }

		/**
		 *	Get the unique identifier.
		 *	@Return unique ID.
		 */
		unsigned int getUID(void) const noexcept { return this->uid; }

		virtual bool operator==(const UIDObject &o1) noexcept { return uid == o1.uid; }

		virtual bool operator!=(const UIDObject &o1) noexcept { return uid == o1.uid; }

		static UIDGenerator &getGenerator(void) noexcept { return generator; }

	  private: /*	Attributes.	*/
		static UIDGenerator generator;
		unsigned int uid; /*	Unique Identifier.	*/
	};
} // namespace MIMIIMGUI

#endif
