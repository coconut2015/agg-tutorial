/*
 * Copyright (c) 2018 Heng Yuan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TUTORIAL_EXCEPTION_H
#define TUTORIAL_EXCEPTION_H

#include <cstdarg>
#include <cstdio>

class TutorialException
{
public:
    TutorialException (const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        vsnprintf(m_msg, sizeof(m_msg), msg, args);
        va_end(args);
    }

    const char* getMessage () const { return m_msg; }
private:
    char    m_msg[257];
};

#define ASSERT(a) do { if (!(a)) throw TutorialException ("Assertion error at %s:%d", __FILE__, __LINE__); } while (0)

#endif  /* TUTORIAL_EXCEPTION_H_ */
