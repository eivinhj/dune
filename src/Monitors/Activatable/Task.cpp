//***************************************************************************
// Copyright 2007-2020 Universidade do Porto - Faculdade de Engenharia      *
// Laboratório de Sistemas e Tecnologia Subaquática (LSTS)                  *
//***************************************************************************
// This file is part of DUNE: Unified Navigation Environment.               *
//                                                                          *
// Commercial Licence Usage                                                 *
// Licencees holding valid commercial DUNE licences may use this file in    *
// accordance with the commercial licence agreement provided with the       *
// Software or, alternatively, in accordance with the terms contained in a  *
// written agreement between you and Faculdade de Engenharia da             *
// Universidade do Porto. For licensing terms, conditions, and further      *
// information contact lsts@fe.up.pt.                                       *
//                                                                          *
// Modified European Union Public Licence - EUPL v.1.1 Usage                *
// Alternatively, this file may be used under the terms of the Modified     *
// EUPL, Version 1.1 only (the "Licence"), appearing in the file LICENCE.md *
// included in the packaging of this file. You may not use this work        *
// except in compliance with the Licence. Unless required by applicable     *
// law or agreed to in writing, software distributed under the Licence is   *
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF     *
// ANY KIND, either express or implied. See the Licence for the specific    *
// language governing permissions and limitations at                        *
// https://github.com/LSTS/dune/blob/master/LICENCE.md and                  *
// http://ec.europa.eu/idabc/eupl.html.                                     *
//***************************************************************************
// Author: Eivind Jølsgard                                                 *
//***************************************************************************

// DUNE headers.
#include <DUNE/DUNE.hpp>

namespace Monitors
{
  //! Insert short task description here.
  //!
  //! Insert explanation on task behaviour here.
  //! @author Eivind Jølsgard
  namespace Activatable
  {
    using DUNE_NAMESPACES;

    struct Arguments
    {
      int limit;
    };

    struct Task: public DUNE::Tasks::Task
    {
      //Public variables
      int message_counter;

      //Arguments
      Arguments message_arguments;


      //! Constructor.
      //! @param[in] name task name.
      //! @param[in] ctx context.
      Task(const std::string& name, Tasks::Context& ctx):
        DUNE::Tasks::Task(name, ctx)
      {
        param("Announces to print", message_arguments.limit)
        //.defaultValue(0)
        .description("Announces to print"); 
        paramActive(Tasks::Parameter::SCOPE_GLOBAL,
                    Tasks::Parameter::VISIBILITY_USER,
                    true);
        message_counter = 0;
        bind<IMC::Announce>(this);
        
      }

      //Activation 
      void
      onActivation(void)
      {
          setEntityState(IMC::EntityState::ESTA_NORMAL, Status::CODE_ACTIVE);
          war("Received %d Announce messages\n\r", message_arguments.limit);
      }   
      //Deactivation
      void
      onDeactivation(void)
      {
         war("Message counter: %d",message_counter);
         setEntityState(IMC::EntityState::ESTA_NORMAL, Status::CODE_IDLE);
         
      }

      //! Update internal state with new parameter values.
      void
      onUpdateParameters(void)
      {
      }

      //! Reserve entity identifiers.
      void
      onEntityReservation(void)
      {
      }

      //! Resolve entity names.
      void
      onEntityResolution(void)
      {
      }

      //! Acquire resources.
      void
      onResourceAcquisition(void)
      {
      }

      //! Initialize resources.
      void
      onResourceInitialization(void)
      {
      }

      //! Release resources.
      void
      onResourceRelease(void)
      {
        spew("Deactivating Activatable");
      }

      void consume(const IMC::Announce* msg)
      {
        if(isActive())
        {
          inf("Message counter: %d\n\r", message_counter);
          message_counter++;
          if(message_counter == message_arguments.limit) 
          {
            requestDeactivation();
            inf("Request deactivation for Activatable");
          }
         // msg->toText(std::cout);
        }
      }

      //! Main loop.
      void
      onMain(void)
      {
        while (!stopping())
        {         
          waitForMessages(1.0);
        }
      }
    };
  }
}

DUNE_TASK
