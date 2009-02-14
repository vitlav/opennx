// $Id$
//
// Copyright (C) 2006 The OpenNX Team
// Author: Fritz Elfert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef _CARDWAITER_H_
#define _CARDWAITER_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "CardWaiter.cpp"
#endif

class wxWindow;

DECLARE_LOCAL_EVENT_TYPE(wxEVT_CARDINSERTED, -5);

/**
 * CardWaiter uses opensc (http://www.opensc-project.org/) to access
 * local smart cards. It watches for insertion of a smart card
 * and shows a message box for instructing the user to insert a
 * smart card if none is found.
 */
class CardWaiter
{
    public:
        CardWaiter(wxWindow *);

        /*
         * Waits fro insertion of a smart card.
         * Returns immediately, if a card is already
         * inserted. Otherwise, shows a dialog which instructs
         * the user to insert a card. The user can hit a Cancel
         * button in that dialog to abort the transaction.
         *
         * @return: -1 on abort or error. The reader-number where
         * the card was inserted otherwise.
         */
        int WaitForCard();

    private:
        wxWindow *parent;
};

#endif
