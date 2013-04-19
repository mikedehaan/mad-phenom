/*
This file is part of mad-phenom.

mad-phenom is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mad-phenom is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with mad-phenom.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MENU_H_
#define MENU_H_

void handleConfig();

#ifdef DWELL_DEBOUNCE
void advancedConfig();
#endif

void successBlink();
void failureBlink();
void configTriggerPulled(uint32_t buttonHeldTime);

#endif /* MENU_H_ */