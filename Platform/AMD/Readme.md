# AMD EDK2 Platform

This is AMD folder that provides the edk2 modules to support AMD edk2 firmware
solution for the server, client (e.g., Notebook) and gaming console platforms.
The board packages under this folder are the firmware reference code for booting
certain AMD platforms. The definition of sub-folders is described in below sections.

## Terminologies

* AMD Platform (platform in short)

  AMD platform refers to a platform that supports the particular AMD SoC (processor), such as
  AMD EPYC Turin processors.

* AMD Board (board in short)

  AMD board is a generic terminology refers to a board that is designed based on a
  specific AMD SoC architecture (or AMD platform). More than one boards are possibly
  designed to support an AMD platform with different configuration, such as 1-processor socket
  or 2-processor sockets board.

* AMD Board package (board package in short)

  The folder has the edk2 meta files to build the necessary edk2 firmware modules
  and generate the binary to run on a board.

## Package Definition

* AmdPlatformPkg

  AMD platform edk2 package under this folder provides the common edk2 modules those
  are leverage by AMD platforms. Usually those modules have no dependencies with
  particular AMD platforms. The module under this scope can provides a common implementation
  for all AMD platforms, or it may just provide a framework but the differences of implementation
  among the platforms could be configured through the PCDs declared in AmdPlatformPkg.dec, or the board level library provided in the \<Board name\>Pkg.

* AmdMinBoardPkg

  This package provides the common edk2 modules those can be leverage across the MinPlatformPkg framework based AMD boards

* \<SoC name\>Board

  This is the folder named by SoC and accommodate one or multiple SKU of board packages those are
  designed base on the SoC platform. <SoC name>Board folder may contain edk2 package meta files 
  directly or the sub-folders named by \<Board name\>Pkg for a variety SKUs of platform.

* <Board name\>Pkg

  This is the folder that contains edk2 package meta files for a board which is designed base on
  a SoC platform. Besides the edk2 meta files, <Board name\>Pkg may also provides edk2 modules
  which are specifically to a board.

  ```
  e.g. OverdriveBoard
  e.g. TurinBoard
           |------Sku1Pkg
           |------Sku2Pkg
  ```

  Below is the outline of folder structure under Platform/AMD

  ```
  Platform/AMD
            |----AmdPlatformPkg
            |----AmdMinBoardPkg
            |----OverdriveBoard
            |----TurinBoard
            |         |------Common Modules for Turin boards
            |         |------Sku1Pkg
            |         |        |-------Board specific modules
            |         |------Sku2Pkg
            |
            |----NextGenBoard
                      |------Common Modules for the next generation
                             platform boards
                      |------Sku1Pkg
                      |------Sku2Pkg
                               |-------Board specific modules
  ```


## Board Support
Under progress