pragma Ada_95;
with System;
package ada_main is
   pragma Warnings (Off);

   gnat_argc : Integer;
   gnat_argv : System.Address;
   gnat_envp : System.Address;

   pragma Import (C, gnat_argc);
   pragma Import (C, gnat_argv);
   pragma Import (C, gnat_envp);

   gnat_exit_status : Integer;
   pragma Import (C, gnat_exit_status);

   GNAT_Version : constant String :=
                    "GNAT Version: 4.8" & ASCII.NUL;
   pragma Export (C, GNAT_Version, "__gnat_version");

   Ada_Main_Program_Name : constant String := "_ada_company" & ASCII.NUL;
   pragma Export (C, Ada_Main_Program_Name, "__gnat_ada_main_program_name");

   procedure adainit;
   pragma Export (C, adainit, "adainit");

   procedure adafinal;
   pragma Export (C, adafinal, "adafinal");

   function main
     (argc : Integer;
      argv : System.Address;
      envp : System.Address)
      return Integer;
   pragma Export (C, main, "main");

   type Version_32 is mod 2 ** 32;
   u00001 : constant Version_32 := 16#ae28e7d1#;
   pragma Export (C, u00001, "companyB");
   u00002 : constant Version_32 := 16#3935bd10#;
   pragma Export (C, u00002, "system__standard_libraryB");
   u00003 : constant Version_32 := 16#b31646c6#;
   pragma Export (C, u00003, "system__standard_libraryS");
   u00004 : constant Version_32 := 16#3ffc8e18#;
   pragma Export (C, u00004, "adaS");
   u00005 : constant Version_32 := 16#45724809#;
   pragma Export (C, u00005, "ada__calendar__delaysB");
   u00006 : constant Version_32 := 16#474dd4b1#;
   pragma Export (C, u00006, "ada__calendar__delaysS");
   u00007 : constant Version_32 := 16#8ba0787e#;
   pragma Export (C, u00007, "ada__calendarB");
   u00008 : constant Version_32 := 16#e791e294#;
   pragma Export (C, u00008, "ada__calendarS");
   u00009 : constant Version_32 := 16#0381b3eb#;
   pragma Export (C, u00009, "ada__exceptionsB");
   u00010 : constant Version_32 := 16#813e0b0c#;
   pragma Export (C, u00010, "ada__exceptionsS");
   u00011 : constant Version_32 := 16#16173147#;
   pragma Export (C, u00011, "ada__exceptions__last_chance_handlerB");
   u00012 : constant Version_32 := 16#1f42fb5e#;
   pragma Export (C, u00012, "ada__exceptions__last_chance_handlerS");
   u00013 : constant Version_32 := 16#bd760655#;
   pragma Export (C, u00013, "systemS");
   u00014 : constant Version_32 := 16#0071025c#;
   pragma Export (C, u00014, "system__soft_linksB");
   u00015 : constant Version_32 := 16#d02d7c88#;
   pragma Export (C, u00015, "system__soft_linksS");
   u00016 : constant Version_32 := 16#27940d94#;
   pragma Export (C, u00016, "system__parametersB");
   u00017 : constant Version_32 := 16#0b940a95#;
   pragma Export (C, u00017, "system__parametersS");
   u00018 : constant Version_32 := 16#17775d6d#;
   pragma Export (C, u00018, "system__secondary_stackB");
   u00019 : constant Version_32 := 16#a91821fb#;
   pragma Export (C, u00019, "system__secondary_stackS");
   u00020 : constant Version_32 := 16#ace32e1e#;
   pragma Export (C, u00020, "system__storage_elementsB");
   u00021 : constant Version_32 := 16#47bb7bcd#;
   pragma Export (C, u00021, "system__storage_elementsS");
   u00022 : constant Version_32 := 16#4f750b3b#;
   pragma Export (C, u00022, "system__stack_checkingB");
   u00023 : constant Version_32 := 16#1ed4ba79#;
   pragma Export (C, u00023, "system__stack_checkingS");
   u00024 : constant Version_32 := 16#7b9f0bae#;
   pragma Export (C, u00024, "system__exception_tableB");
   u00025 : constant Version_32 := 16#2c18daf0#;
   pragma Export (C, u00025, "system__exception_tableS");
   u00026 : constant Version_32 := 16#5665ab64#;
   pragma Export (C, u00026, "system__htableB");
   u00027 : constant Version_32 := 16#3ede485b#;
   pragma Export (C, u00027, "system__htableS");
   u00028 : constant Version_32 := 16#8b7dad61#;
   pragma Export (C, u00028, "system__string_hashB");
   u00029 : constant Version_32 := 16#9beadec1#;
   pragma Export (C, u00029, "system__string_hashS");
   u00030 : constant Version_32 := 16#aad75561#;
   pragma Export (C, u00030, "system__exceptionsB");
   u00031 : constant Version_32 := 16#b188cee2#;
   pragma Export (C, u00031, "system__exceptionsS");
   u00032 : constant Version_32 := 16#010db1dc#;
   pragma Export (C, u00032, "system__exceptions_debugB");
   u00033 : constant Version_32 := 16#85062381#;
   pragma Export (C, u00033, "system__exceptions_debugS");
   u00034 : constant Version_32 := 16#b012ff50#;
   pragma Export (C, u00034, "system__img_intB");
   u00035 : constant Version_32 := 16#bfade697#;
   pragma Export (C, u00035, "system__img_intS");
   u00036 : constant Version_32 := 16#dc8e33ed#;
   pragma Export (C, u00036, "system__tracebackB");
   u00037 : constant Version_32 := 16#dcf1d220#;
   pragma Export (C, u00037, "system__tracebackS");
   u00038 : constant Version_32 := 16#907d882f#;
   pragma Export (C, u00038, "system__wch_conB");
   u00039 : constant Version_32 := 16#029d2868#;
   pragma Export (C, u00039, "system__wch_conS");
   u00040 : constant Version_32 := 16#22fed88a#;
   pragma Export (C, u00040, "system__wch_stwB");
   u00041 : constant Version_32 := 16#2f8c0469#;
   pragma Export (C, u00041, "system__wch_stwS");
   u00042 : constant Version_32 := 16#b8a9e30d#;
   pragma Export (C, u00042, "system__wch_cnvB");
   u00043 : constant Version_32 := 16#1c63aebe#;
   pragma Export (C, u00043, "system__wch_cnvS");
   u00044 : constant Version_32 := 16#129923ea#;
   pragma Export (C, u00044, "interfacesS");
   u00045 : constant Version_32 := 16#75729fba#;
   pragma Export (C, u00045, "system__wch_jisB");
   u00046 : constant Version_32 := 16#481135aa#;
   pragma Export (C, u00046, "system__wch_jisS");
   u00047 : constant Version_32 := 16#ada34a87#;
   pragma Export (C, u00047, "system__traceback_entriesB");
   u00048 : constant Version_32 := 16#ef57e814#;
   pragma Export (C, u00048, "system__traceback_entriesS");
   u00049 : constant Version_32 := 16#769e25e6#;
   pragma Export (C, u00049, "interfaces__cB");
   u00050 : constant Version_32 := 16#f05a3eb1#;
   pragma Export (C, u00050, "interfaces__cS");
   u00051 : constant Version_32 := 16#22d03640#;
   pragma Export (C, u00051, "system__os_primitivesB");
   u00052 : constant Version_32 := 16#0da78a7c#;
   pragma Export (C, u00052, "system__os_primitivesS");
   u00053 : constant Version_32 := 16#ee80728a#;
   pragma Export (C, u00053, "system__tracesB");
   u00054 : constant Version_32 := 16#4f6b6eff#;
   pragma Export (C, u00054, "system__tracesS");
   u00055 : constant Version_32 := 16#84ad4a42#;
   pragma Export (C, u00055, "ada__numericsS");
   u00056 : constant Version_32 := 16#ac5daf3d#;
   pragma Export (C, u00056, "ada__numerics__float_randomB");
   u00057 : constant Version_32 := 16#ac27f55b#;
   pragma Export (C, u00057, "ada__numerics__float_randomS");
   u00058 : constant Version_32 := 16#8053c412#;
   pragma Export (C, u00058, "system__random_numbersB");
   u00059 : constant Version_32 := 16#f15c1ab5#;
   pragma Export (C, u00059, "system__random_numbersS");
   u00060 : constant Version_32 := 16#194ccd7b#;
   pragma Export (C, u00060, "system__img_unsB");
   u00061 : constant Version_32 := 16#486366d4#;
   pragma Export (C, u00061, "system__img_unsS");
   u00062 : constant Version_32 := 16#d7975a23#;
   pragma Export (C, u00062, "system__unsigned_typesS");
   u00063 : constant Version_32 := 16#7d397bc7#;
   pragma Export (C, u00063, "system__random_seedB");
   u00064 : constant Version_32 := 16#ae4a5e8c#;
   pragma Export (C, u00064, "system__random_seedS");
   u00065 : constant Version_32 := 16#79817c71#;
   pragma Export (C, u00065, "system__val_unsB");
   u00066 : constant Version_32 := 16#c73fb718#;
   pragma Export (C, u00066, "system__val_unsS");
   u00067 : constant Version_32 := 16#aea309ed#;
   pragma Export (C, u00067, "system__val_utilB");
   u00068 : constant Version_32 := 16#11d6b0ab#;
   pragma Export (C, u00068, "system__val_utilS");
   u00069 : constant Version_32 := 16#b7fa72e7#;
   pragma Export (C, u00069, "system__case_utilB");
   u00070 : constant Version_32 := 16#106a66dd#;
   pragma Export (C, u00070, "system__case_utilS");
   u00071 : constant Version_32 := 16#ebdf2801#;
   pragma Export (C, u00071, "ada__real_timeB");
   u00072 : constant Version_32 := 16#41de19c7#;
   pragma Export (C, u00072, "ada__real_timeS");
   u00073 : constant Version_32 := 16#7384e8d6#;
   pragma Export (C, u00073, "system__arith_64B");
   u00074 : constant Version_32 := 16#efa2804d#;
   pragma Export (C, u00074, "system__arith_64S");
   u00075 : constant Version_32 := 16#52c41c4b#;
   pragma Export (C, u00075, "system__taskingB");
   u00076 : constant Version_32 := 16#92b38721#;
   pragma Export (C, u00076, "system__taskingS");
   u00077 : constant Version_32 := 16#3b0b7afe#;
   pragma Export (C, u00077, "system__task_primitivesS");
   u00078 : constant Version_32 := 16#e0522444#;
   pragma Export (C, u00078, "system__os_interfaceB");
   u00079 : constant Version_32 := 16#25d7c152#;
   pragma Export (C, u00079, "system__os_interfaceS");
   u00080 : constant Version_32 := 16#cdb1cd10#;
   pragma Export (C, u00080, "system__linuxS");
   u00081 : constant Version_32 := 16#3f70ee12#;
   pragma Export (C, u00081, "system__os_constantsS");
   u00082 : constant Version_32 := 16#b4921ea2#;
   pragma Export (C, u00082, "system__task_primitives__operationsB");
   u00083 : constant Version_32 := 16#74a4c38b#;
   pragma Export (C, u00083, "system__task_primitives__operationsS");
   u00084 : constant Version_32 := 16#35042ca6#;
   pragma Export (C, u00084, "system__bit_opsB");
   u00085 : constant Version_32 := 16#c30e4013#;
   pragma Export (C, u00085, "system__bit_opsS");
   u00086 : constant Version_32 := 16#903909a4#;
   pragma Export (C, u00086, "system__interrupt_managementB");
   u00087 : constant Version_32 := 16#3d6662cb#;
   pragma Export (C, u00087, "system__interrupt_managementS");
   u00088 : constant Version_32 := 16#f65595cf#;
   pragma Export (C, u00088, "system__multiprocessorsB");
   u00089 : constant Version_32 := 16#85da9926#;
   pragma Export (C, u00089, "system__multiprocessorsS");
   u00090 : constant Version_32 := 16#e7299166#;
   pragma Export (C, u00090, "system__stack_checking__operationsB");
   u00091 : constant Version_32 := 16#49df1cef#;
   pragma Export (C, u00091, "system__stack_checking__operationsS");
   u00092 : constant Version_32 := 16#1530815b#;
   pragma Export (C, u00092, "system__crtlS");
   u00093 : constant Version_32 := 16#3d54d5f6#;
   pragma Export (C, u00093, "system__task_infoB");
   u00094 : constant Version_32 := 16#0fcc0405#;
   pragma Export (C, u00094, "system__task_infoS");
   u00095 : constant Version_32 := 16#1872c2a7#;
   pragma Export (C, u00095, "system__tasking__debugB");
   u00096 : constant Version_32 := 16#8c562538#;
   pragma Export (C, u00096, "system__tasking__debugS");
   u00097 : constant Version_32 := 16#39591e91#;
   pragma Export (C, u00097, "system__concat_2B");
   u00098 : constant Version_32 := 16#46a6f4a9#;
   pragma Export (C, u00098, "system__concat_2S");
   u00099 : constant Version_32 := 16#ae97ef6c#;
   pragma Export (C, u00099, "system__concat_3B");
   u00100 : constant Version_32 := 16#cb5c043f#;
   pragma Export (C, u00100, "system__concat_3S");
   u00101 : constant Version_32 := 16#c9fdc962#;
   pragma Export (C, u00101, "system__concat_6B");
   u00102 : constant Version_32 := 16#7abcf341#;
   pragma Export (C, u00102, "system__concat_6S");
   u00103 : constant Version_32 := 16#def1dd00#;
   pragma Export (C, u00103, "system__concat_5B");
   u00104 : constant Version_32 := 16#ad4fc8f4#;
   pragma Export (C, u00104, "system__concat_5S");
   u00105 : constant Version_32 := 16#3493e6c0#;
   pragma Export (C, u00105, "system__concat_4B");
   u00106 : constant Version_32 := 16#bf29e5eb#;
   pragma Export (C, u00106, "system__concat_4S");
   u00107 : constant Version_32 := 16#1eab0e09#;
   pragma Export (C, u00107, "system__img_enum_newB");
   u00108 : constant Version_32 := 16#3a71cda5#;
   pragma Export (C, u00108, "system__img_enum_newS");
   u00109 : constant Version_32 := 16#c959d75d#;
   pragma Export (C, u00109, "system__stack_usageB");
   u00110 : constant Version_32 := 16#a5188558#;
   pragma Export (C, u00110, "system__stack_usageS");
   u00111 : constant Version_32 := 16#d7aac20c#;
   pragma Export (C, u00111, "system__ioB");
   u00112 : constant Version_32 := 16#2334f11a#;
   pragma Export (C, u00112, "system__ioS");
   u00113 : constant Version_32 := 16#afd62b40#;
   pragma Export (C, u00113, "ada__tagsB");
   u00114 : constant Version_32 := 16#1442fc05#;
   pragma Export (C, u00114, "ada__tagsS");
   u00115 : constant Version_32 := 16#30d8d53e#;
   pragma Export (C, u00115, "ada__text_ioB");
   u00116 : constant Version_32 := 16#f792461d#;
   pragma Export (C, u00116, "ada__text_ioS");
   u00117 : constant Version_32 := 16#1358602f#;
   pragma Export (C, u00117, "ada__streamsS");
   u00118 : constant Version_32 := 16#53190229#;
   pragma Export (C, u00118, "interfaces__c_streamsB");
   u00119 : constant Version_32 := 16#3ebb5e8e#;
   pragma Export (C, u00119, "interfaces__c_streamsS");
   u00120 : constant Version_32 := 16#35bbd729#;
   pragma Export (C, u00120, "system__file_ioB");
   u00121 : constant Version_32 := 16#9f99c2b3#;
   pragma Export (C, u00121, "system__file_ioS");
   u00122 : constant Version_32 := 16#8cbe6205#;
   pragma Export (C, u00122, "ada__finalizationB");
   u00123 : constant Version_32 := 16#22e22193#;
   pragma Export (C, u00123, "ada__finalizationS");
   u00124 : constant Version_32 := 16#95817ed8#;
   pragma Export (C, u00124, "system__finalization_rootB");
   u00125 : constant Version_32 := 16#f28475c5#;
   pragma Export (C, u00125, "system__finalization_rootS");
   u00126 : constant Version_32 := 16#b46168d5#;
   pragma Export (C, u00126, "ada__io_exceptionsS");
   u00127 : constant Version_32 := 16#b2cb9bcf#;
   pragma Export (C, u00127, "interfaces__c__stringsB");
   u00128 : constant Version_32 := 16#603c1c44#;
   pragma Export (C, u00128, "interfaces__c__stringsS");
   u00129 : constant Version_32 := 16#17d70f63#;
   pragma Export (C, u00129, "system__crtl__runtimeS");
   u00130 : constant Version_32 := 16#d53b6c45#;
   pragma Export (C, u00130, "system__os_libB");
   u00131 : constant Version_32 := 16#89dce9aa#;
   pragma Export (C, u00131, "system__os_libS");
   u00132 : constant Version_32 := 16#4cd8aca0#;
   pragma Export (C, u00132, "system__stringsB");
   u00133 : constant Version_32 := 16#0a9c4c91#;
   pragma Export (C, u00133, "system__stringsS");
   u00134 : constant Version_32 := 16#e8578845#;
   pragma Export (C, u00134, "system__file_control_blockS");
   u00135 : constant Version_32 := 16#91d2300e#;
   pragma Export (C, u00135, "system__finalization_mastersB");
   u00136 : constant Version_32 := 16#d783aa79#;
   pragma Export (C, u00136, "system__finalization_mastersS");
   u00137 : constant Version_32 := 16#57a37a42#;
   pragma Export (C, u00137, "system__address_imageB");
   u00138 : constant Version_32 := 16#1c9a9b6f#;
   pragma Export (C, u00138, "system__address_imageS");
   u00139 : constant Version_32 := 16#7268f812#;
   pragma Export (C, u00139, "system__img_boolB");
   u00140 : constant Version_32 := 16#48af77be#;
   pragma Export (C, u00140, "system__img_boolS");
   u00141 : constant Version_32 := 16#a7a37cb6#;
   pragma Export (C, u00141, "system__storage_poolsB");
   u00142 : constant Version_32 := 16#6ed81938#;
   pragma Export (C, u00142, "system__storage_poolsS");
   u00143 : constant Version_32 := 16#ba5d60c7#;
   pragma Export (C, u00143, "system__pool_globalB");
   u00144 : constant Version_32 := 16#d56df0a6#;
   pragma Export (C, u00144, "system__pool_globalS");
   u00145 : constant Version_32 := 16#3d0b17cc#;
   pragma Export (C, u00145, "system__memoryB");
   u00146 : constant Version_32 := 16#77fdba40#;
   pragma Export (C, u00146, "system__memoryS");
   u00147 : constant Version_32 := 16#1fd820b1#;
   pragma Export (C, u00147, "system__storage_pools__subpoolsB");
   u00148 : constant Version_32 := 16#951e0de9#;
   pragma Export (C, u00148, "system__storage_pools__subpoolsS");
   u00149 : constant Version_32 := 16#1777d351#;
   pragma Export (C, u00149, "system__storage_pools__subpools__finalizationB");
   u00150 : constant Version_32 := 16#12aaf1de#;
   pragma Export (C, u00150, "system__storage_pools__subpools__finalizationS");
   u00151 : constant Version_32 := 16#2b72f470#;
   pragma Export (C, u00151, "definitionsS");
   u00152 : constant Version_32 := 16#2b93a046#;
   pragma Export (C, u00152, "system__img_charB");
   u00153 : constant Version_32 := 16#21425eb2#;
   pragma Export (C, u00153, "system__img_charS");
   u00154 : constant Version_32 := 16#da47006c#;
   pragma Export (C, u00154, "system__tasking__rendezvousB");
   u00155 : constant Version_32 := 16#592e9c02#;
   pragma Export (C, u00155, "system__tasking__rendezvousS");
   u00156 : constant Version_32 := 16#386436bc#;
   pragma Export (C, u00156, "system__restrictionsB");
   u00157 : constant Version_32 := 16#f1bbb6dc#;
   pragma Export (C, u00157, "system__restrictionsS");
   u00158 : constant Version_32 := 16#f81293e6#;
   pragma Export (C, u00158, "system__tasking__entry_callsB");
   u00159 : constant Version_32 := 16#837d42fa#;
   pragma Export (C, u00159, "system__tasking__entry_callsS");
   u00160 : constant Version_32 := 16#6b472777#;
   pragma Export (C, u00160, "system__tasking__initializationB");
   u00161 : constant Version_32 := 16#9468d5af#;
   pragma Export (C, u00161, "system__tasking__initializationS");
   u00162 : constant Version_32 := 16#12b846a5#;
   pragma Export (C, u00162, "system__soft_links__taskingB");
   u00163 : constant Version_32 := 16#6ac0d6d0#;
   pragma Export (C, u00163, "system__soft_links__taskingS");
   u00164 : constant Version_32 := 16#17d21067#;
   pragma Export (C, u00164, "ada__exceptions__is_null_occurrenceB");
   u00165 : constant Version_32 := 16#d832eaef#;
   pragma Export (C, u00165, "ada__exceptions__is_null_occurrenceS");
   u00166 : constant Version_32 := 16#78928eb3#;
   pragma Export (C, u00166, "system__tasking__protected_objectsB");
   u00167 : constant Version_32 := 16#09cb1bb5#;
   pragma Export (C, u00167, "system__tasking__protected_objectsS");
   u00168 : constant Version_32 := 16#4861433c#;
   pragma Export (C, u00168, "system__tasking__protected_objects__entriesB");
   u00169 : constant Version_32 := 16#4d64e3b6#;
   pragma Export (C, u00169, "system__tasking__protected_objects__entriesS");
   u00170 : constant Version_32 := 16#84c1a39b#;
   pragma Export (C, u00170, "system__tasking__protected_objects__operationsB");
   u00171 : constant Version_32 := 16#a9cb954d#;
   pragma Export (C, u00171, "system__tasking__protected_objects__operationsS");
   u00172 : constant Version_32 := 16#b892e5ab#;
   pragma Export (C, u00172, "system__tasking__queuingB");
   u00173 : constant Version_32 := 16#5b69ac57#;
   pragma Export (C, u00173, "system__tasking__queuingS");
   u00174 : constant Version_32 := 16#d2bce054#;
   pragma Export (C, u00174, "system__tasking__utilitiesB");
   u00175 : constant Version_32 := 16#5f437348#;
   pragma Export (C, u00175, "system__tasking__utilitiesS");
   u00176 : constant Version_32 := 16#bd6fc52e#;
   pragma Export (C, u00176, "system__traces__taskingB");
   u00177 : constant Version_32 := 16#55cf3c43#;
   pragma Export (C, u00177, "system__traces__taskingS");
   u00178 : constant Version_32 := 16#48031dbe#;
   pragma Export (C, u00178, "system__tasking__stagesB");
   u00179 : constant Version_32 := 16#dd4b61af#;
   pragma Export (C, u00179, "system__tasking__stagesS");
   --  BEGIN ELABORATION ORDER
   --  ada%s
   --  interfaces%s
   --  system%s
   --  system.arith_64%s
   --  system.case_util%s
   --  system.case_util%b
   --  system.htable%s
   --  system.img_bool%s
   --  system.img_bool%b
   --  system.img_char%s
   --  system.img_char%b
   --  system.img_enum_new%s
   --  system.img_enum_new%b
   --  system.img_int%s
   --  system.img_int%b
   --  system.io%s
   --  system.io%b
   --  system.linux%s
   --  system.multiprocessors%s
   --  system.os_primitives%s
   --  system.os_primitives%b
   --  system.parameters%s
   --  system.parameters%b
   --  system.crtl%s
   --  interfaces.c_streams%s
   --  interfaces.c_streams%b
   --  system.restrictions%s
   --  system.restrictions%b
   --  system.standard_library%s
   --  system.exceptions_debug%s
   --  system.exceptions_debug%b
   --  system.storage_elements%s
   --  system.storage_elements%b
   --  system.stack_checking%s
   --  system.stack_checking%b
   --  system.stack_checking.operations%s
   --  system.stack_usage%s
   --  system.stack_usage%b
   --  system.string_hash%s
   --  system.string_hash%b
   --  system.htable%b
   --  system.strings%s
   --  system.strings%b
   --  system.os_lib%s
   --  system.traceback_entries%s
   --  system.traceback_entries%b
   --  ada.exceptions%s
   --  system.arith_64%b
   --  ada.exceptions.is_null_occurrence%s
   --  ada.exceptions.is_null_occurrence%b
   --  system.soft_links%s
   --  system.stack_checking.operations%b
   --  system.traces%s
   --  system.traces%b
   --  system.unsigned_types%s
   --  system.img_uns%s
   --  system.img_uns%b
   --  system.val_uns%s
   --  system.val_util%s
   --  system.val_util%b
   --  system.val_uns%b
   --  system.wch_con%s
   --  system.wch_con%b
   --  system.wch_cnv%s
   --  system.wch_jis%s
   --  system.wch_jis%b
   --  system.wch_cnv%b
   --  system.wch_stw%s
   --  system.wch_stw%b
   --  ada.exceptions.last_chance_handler%s
   --  ada.exceptions.last_chance_handler%b
   --  system.address_image%s
   --  system.bit_ops%s
   --  system.bit_ops%b
   --  system.concat_2%s
   --  system.concat_2%b
   --  system.concat_3%s
   --  system.concat_3%b
   --  system.concat_4%s
   --  system.concat_4%b
   --  system.concat_5%s
   --  system.concat_5%b
   --  system.concat_6%s
   --  system.concat_6%b
   --  system.exception_table%s
   --  system.exception_table%b
   --  ada.io_exceptions%s
   --  ada.numerics%s
   --  ada.tags%s
   --  ada.streams%s
   --  interfaces.c%s
   --  system.multiprocessors%b
   --  interfaces.c.strings%s
   --  system.crtl.runtime%s
   --  system.exceptions%s
   --  system.exceptions%b
   --  system.finalization_root%s
   --  system.finalization_root%b
   --  ada.finalization%s
   --  ada.finalization%b
   --  system.os_constants%s
   --  system.os_interface%s
   --  system.os_interface%b
   --  system.interrupt_management%s
   --  system.storage_pools%s
   --  system.storage_pools%b
   --  system.finalization_masters%s
   --  system.storage_pools.subpools%s
   --  system.storage_pools.subpools.finalization%s
   --  system.storage_pools.subpools.finalization%b
   --  system.task_info%s
   --  system.task_info%b
   --  system.task_primitives%s
   --  system.interrupt_management%b
   --  system.tasking%s
   --  system.task_primitives.operations%s
   --  system.tasking%b
   --  system.tasking.debug%s
   --  system.tasking.debug%b
   --  system.task_primitives.operations%b
   --  system.traces.tasking%s
   --  system.traces.tasking%b
   --  ada.calendar%s
   --  ada.calendar%b
   --  ada.calendar.delays%s
   --  ada.calendar.delays%b
   --  system.memory%s
   --  system.memory%b
   --  system.standard_library%b
   --  system.pool_global%s
   --  system.pool_global%b
   --  system.file_control_block%s
   --  system.file_io%s
   --  system.random_numbers%s
   --  ada.numerics.float_random%s
   --  ada.numerics.float_random%b
   --  system.random_seed%s
   --  system.random_seed%b
   --  system.secondary_stack%s
   --  system.file_io%b
   --  system.storage_pools.subpools%b
   --  system.finalization_masters%b
   --  interfaces.c.strings%b
   --  interfaces.c%b
   --  ada.tags%b
   --  system.soft_links%b
   --  system.os_lib%b
   --  system.secondary_stack%b
   --  system.random_numbers%b
   --  system.address_image%b
   --  system.soft_links.tasking%s
   --  system.soft_links.tasking%b
   --  system.tasking.entry_calls%s
   --  system.tasking.initialization%s
   --  system.tasking.initialization%b
   --  system.tasking.protected_objects%s
   --  system.tasking.protected_objects%b
   --  system.tasking.utilities%s
   --  system.traceback%s
   --  ada.exceptions%b
   --  system.traceback%b
   --  ada.real_time%s
   --  ada.real_time%b
   --  ada.text_io%s
   --  ada.text_io%b
   --  system.tasking.protected_objects.entries%s
   --  system.tasking.protected_objects.entries%b
   --  system.tasking.queuing%s
   --  system.tasking.queuing%b
   --  system.tasking.utilities%b
   --  system.tasking.rendezvous%s
   --  system.tasking.protected_objects.operations%s
   --  system.tasking.protected_objects.operations%b
   --  system.tasking.rendezvous%b
   --  system.tasking.entry_calls%b
   --  system.tasking.stages%s
   --  system.tasking.stages%b
   --  definitions%s
   --  company%b
   --  END ELABORATION ORDER


end ada_main;
