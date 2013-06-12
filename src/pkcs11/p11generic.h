/**
 * SmartCard-HSM PKCS#11 Module
 *
 * Copyright (c) 2013, CardContact Systems GmbH, Minden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of CardContact Systems GmbH nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CardContact Systems GmbH BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file    p11generic.h
 * @author  Frank Thater, Andreas Schwier
 * @brief   General module functions at the PKCS#11 interface
 */

#ifndef ___P11GENERIC_H_INC___
#define ___P11GENERIC_H_INC___

#include <stdio.h>
#include <stdlib.h>

#include <pkcs11/cryptoki.h>

#include <pkcs11/session.h>
#include <pkcs11/object.h>

#ifndef _MAX_PATH
#define _MAX_PATH FILENAME_MAX
#endif

#ifdef DEBUG
#define FUNC_CALLED() do { \
		debug("Function %s called.\n", __FUNCTION__); \
} while (0)

#define FUNC_RETURNS(rc) do { \
		debug("Function %s completes with rc=%d.\n", __FUNCTION__, (rc)); \
		return rc; \
} while (0)

#define FUNC_FAILS(rc, msg) do { \
		debug("Function %s fails with rc=%d \"%s\"\n", __FUNCTION__, (rc), (msg)); \
		return rc; \
} while (0)

#else
#define FUNC_CALLED()
#define FUNC_RETURNS(rc) return (rc)
#define FUNC_FAILS(rc, msg) return (rc)
#endif

/**
 * Internal structure to store information about a token.
 *
 */

struct p11Token_t {

	CK_TOKEN_INFO info;                 /**< General information about the token            */
	struct p11Slot_t *slot;             /**< The slot where the token is inserted           */
	CK_USER_TYPE user;                  /**< The user of this session                       */
	int rosessions;                     /**< Number of read/only sessions                   */
	CK_ULONG freeObjectNumber;          /**< The number of objects in this token            */

	CK_MECHANISM_TYPE mechanism;        /**< Mechanisms supported by token                  */

	CK_ULONG numberOfTokenObjects;      /**< The number of public objects in this token     */
	struct p11Object_t *tokenObjList;   /**< Pointer to first object in pool                */

	CK_ULONG numberOfPrivateTokenObjects; /**< The number of private objects in this token  */
	struct p11Object_t *tokenPrivObjList; /**< Pointer to the first object in pool          */
};

/**
 * Internal structure to store information about a slot.
 *
 */

struct p11Slot_t {

	CK_SLOT_ID id;                  /**< The id of the slot                  */
	CK_SLOT_INFO info;              /**< General information about the slot  */
	int closed;                     /**< Slot hardware currently absent      */
	char slotDir[_MAX_PATH];        /**< The directory that holds this slot  */

	struct p11Token_t *token;       /**< Pointer to token in the slot        */

	struct p11Slot_t *next;         /**< Pointer to next available slot      */

};

/**
 * Internal structure to store information about all available slots.
 *
 */

struct p11SlotPool_t {

	CK_ULONG numberOfSlots;         /**< Number of slots in the pool         */
	CK_SLOT_ID nextSlotID;          /**< The next assigned slot ID value     */
	struct p11Slot_t *list;         /**< Pointer to first slot in pool       */

};

/**
 * Internal context structure of the cryptoki.
 *
 */

struct p11Context_t {

	CK_VERSION version;                     /**< Information about cryptoki version       */
	CK_INFO info;                           /**< General information about cryptoki       */
	CK_HW_FEATURE_TYPE hw_feature;          /**< Hardware feature type of device          */

	FILE *debugFileHandle;

	struct p11SessionPool_t *sessionPool;   /**< Pointer to session pool                  */

	struct p11SlotPool_t *slotPool;         /**< Pointer to pool of available slots       */

};

#endif /* ___P11GENERIC_H_INC___ */

