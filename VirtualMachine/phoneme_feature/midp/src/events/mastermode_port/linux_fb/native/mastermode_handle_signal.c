/*
 *
 *
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

/**
 * @file
 *
 * Utility functions to handle received system signals.
 */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <midpServices.h>
#include <midpEvents.h>
#include <midpEventUtil.h>
#include <fbapp_export.h>
#include <midp_input_port.h>
#include <midp_logging.h>
#include <pcsl_network_generic.h>
#include <jvm.h>

/**
 * Prepare read/write/exception descriptor sets with data from
 * socket list for suceeded select() query
 *
 * @param socketsList list of sockets registered for read/write notifications
 * @param pRead_fds set of descriptors to check for read signals
 * @param pWrite_fds set of descriptors to check for write signals
 * @param pExcept_fds set of descriptors to check for exception signals
 * @param pNum_fds upper bound of checked descriptor values
 */
void setSockets(const SocketHandle* socketsList,
        /*OUT*/ fd_set* pRead_fds, /*OUT*/ fd_set* pWrite_fds,
        /*OUT*/ fd_set* pExcept_fds, /*OUT*/ int* pNum_fds) {

    if (socketsList != NULL) {
        const SocketHandle *socket = (const SocketHandle *)socketsList;
        for(; socket != NULL; socket = socket->next) {
            if (socket->check_flags & CHECK_READ) {
                FD_SET(socket->fd, pRead_fds);
            }
            if (socket->check_flags & CHECK_WRITE) {
                FD_SET(socket->fd, pWrite_fds);
            }
            FD_SET(socket->fd, pExcept_fds);
            if (*pNum_fds <= socket->fd) {
               *pNum_fds = socket->fd + 1;
            }
        }
    }
}

/**
 * Handle received socket signal and prepare reentry data
 * to unblock a thread waiting for the signal.
 *
 * @param socketsList list of sockets registered for read/write notifications
 * @param pRead_fds set of descriptors to check for read signals
 * @param pWrite_fds set of descriptors to check for write signals
 * @param pExcept_fds set of descriptors to check for exception signals
 * @param pNewSignal reentry data to unblock a threads waiting for a socket signal
 */
void handleSockets(const SocketHandle* socketsList,
        fd_set* pRead_fds, fd_set* pWrite_fds, fd_set* pExcept_fds,
        /*OUT*/ MidpReentryData* pNewSignal) {

    if (socketsList != NULL) {
        /* Handle socket events */
        const SocketHandle *socket = (const SocketHandle *)socketsList;
        for(; socket != NULL; socket = socket->next) {
            if (FD_ISSET(socket->fd, pExcept_fds)) {
                pNewSignal->descriptor = (int)socket;
                pNewSignal->waitingFor = NETWORK_EXCEPTION_SIGNAL;
                break;
            }
            if ((socket->check_flags & CHECK_READ) &&
                    FD_ISSET(socket->fd, pRead_fds)) {
                pNewSignal->descriptor = (int)socket;
                pNewSignal->waitingFor = NETWORK_READ_SIGNAL;
                break;
            }
            if ((socket->check_flags & CHECK_WRITE) &&
                    FD_ISSET(socket->fd, pWrite_fds)) {
                pNewSignal->descriptor = (int)socket;
                pNewSignal->waitingFor = NETWORK_WRITE_SIGNAL;
                break;
            }
        } /* for */
    } /* socketList != NULL */
}

/**
 * Handle received keyboard/keypad signals
 *
 * @param pNewSignal        reentry data to unblock threads waiting for a signal
 * @param pNewMidpEvent     a native MIDP event to be stored to Java event queue
 */
void handleKey(MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent) {
    handle_key_port(pNewSignal, pNewMidpEvent);
}

/**
 * Handle received pointer signals
 *
 * @param pNewSignal        reentry data to unblock threads waiting for a signal
 * @param pNewMidpEvent     a native MIDP event to be stored to Java event queue
 */
void handlePointer(MidpReentryData* pNewSignal, MidpEvent* pNewMidpEvent) {
    handle_pointer_port(pNewSignal, pNewMidpEvent);
}
    
/**
 * An input devices can produce bit-based keyboard events. Thus single
 * native event can produce several MIDP ones. This function detects
 * whether are one or more key bits still not converted into MIDP events
 *
 * @return true when pending key exists, false otherwise
 */
jboolean hasPendingKey() {
    return has_pending_key_port();
}
