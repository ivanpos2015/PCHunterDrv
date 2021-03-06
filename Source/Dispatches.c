#include "Dispatches.h"


NTSTATUS
IoControlPassThrough(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS			Status = STATUS_SUCCESS;
	PVOID				InputBuffer = NULL;
	PVOID               OutputBuffer = NULL;
	UINT32				InputBufferLength = 0;
	UINT32				OutputBufferLength = 0;
	PIO_STACK_LOCATION	IrpStack;
	UINT32				IoControlCode;

	IrpStack = IoGetCurrentIrpStackLocation(Irp);		// ��õ�ǰIrp��ջ
	InputBuffer = IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
	OutputBuffer = Irp->UserBuffer;
	InputBufferLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
	OutputBufferLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	switch (IrpStack->MajorFunction)
	{
	case IRP_MJ_DEVICE_CONTROL:
	{
		IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
		switch (IoControlCode)
		{
		//////////////////////////////////////////////////////////////////////////
		// ProcessCore
		case IOCTL_PROC_SEND_SELF_PID:
		{
			DbgPrint("Send Self Pid\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT32));

					Status = SetSelfProcessId(*(PUINT32)InputBuffer, OutputBuffer, &OutputBufferLength);

					Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_GET_PROCESS_COUNT:
		{
			DbgPrint("Get Process Count\r\n");

			__try
			{
				ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT32));

				Status = GetSystemProcessCount(OutputBuffer, &OutputBufferLength);

				Irp->IoStatus.Information = OutputBufferLength;
				Irp->IoStatus.Status = Status;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("Catch Exception\r\n");
				Status = STATUS_UNSUCCESSFUL;
			}

			break;
		}
		case IOCTL_PROC_ENUM_PROCESS_LIST:
		{
			DbgPrint("Enum Process List\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

					Status = EnumSystemProcessList(*(PUINT32)InputBuffer, OutputBuffer, &OutputBufferLength);

					Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PROCESS_THREAD:
		{
			DbgPrint("Process Thread\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

					Status = EnumProcessThread(*(PUINT32)InputBuffer, OutputBuffer, OutputBufferLength);

					//Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PROCESS_MODULE:
		{
			DbgPrint("Process Module\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

					Status = EnumProcessModule(*(PUINT32)InputBuffer, OutputBuffer, OutputBufferLength);

					//Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}

		case IOCTL_PROC_PROCESS_PRIVILEGE:
		{
			DbgPrint("Process Privilege\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

					Status = EnumProcessPrivilege(*(PUINT32)InputBuffer, OutputBuffer, OutputBufferLength);

					//Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PRIVILEGE_ADJUST:
		{
			DbgPrint("Privilege Adjust\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT8));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

					Status = AdjustProcessTokenPrivileges((PPRIVILEGE_DATA)InputBuffer, (int*)OutputBuffer);

					//Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PROCESS_HANDLE:
		{
			DbgPrint("Process Handle\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(PVOID));

					Status = EnumProcessHandle(*(PUINT32)InputBuffer, OutputBuffer, OutputBufferLength);

					Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PROCESS_WINDOW:
		{
			DbgPrint("Process Window\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(PVOID));

					Status = EnumProcessWindow(*(PUINT32)InputBuffer, OutputBuffer, OutputBufferLength);

					Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PROCESS_MEMORY:
		{
			DbgPrint("Process Memory\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(PVOID));

					Status = EnumProcessMemory(*(PUINT32)InputBuffer, OutputBuffer, OutputBufferLength);

					Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		case IOCTL_PROC_PROCESS_KILL:
		{
			DbgPrint("Kill Process\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT32));
					ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(INT32));

					Status = KillProcess(*(PUINT32)InputBuffer, OutputBuffer);

					Irp->IoStatus.Information = OutputBufferLength;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}

			break;
		}
		//////////////////////////////////////////////////////////////////////////
		// ModuleCore
		case IOCTL_MODU_ENUM_MODULE_LIST:
		{
			DbgPrint("Enum Module\r\n");

			__try
			{
				ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

				Status = EnumSystemModuleList(OutputBuffer, OutputBufferLength);

				Irp->IoStatus.Information = OutputBufferLength;
				Irp->IoStatus.Status = Status;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("Catch Exception\r\n");
				Status = STATUS_UNSUCCESSFUL;
			}

			break;
		}
		case IOCTL_MODU_UNLOAD_MODULE:
		{
			DbgPrint("Unload Module\r\n");

			if (InputBufferLength >= sizeof(UINT32) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(PVOID));

					Status = UnloadDriverObject((PVOID)*(PUINT_PTR)InputBuffer, InputBufferLength);

					Irp->IoStatus.Information = 0;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		// KernelSys
		case IOCTL_SYS_ENUM_CALLBACK_LIST:
		{
			DbgPrint("Enum Callbacks\r\n");
			__try
			{
				ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

				Status = EnumSysCallbackNotify(OutputBuffer, OutputBufferLength);

				Irp->IoStatus.Information = 0;
				Irp->IoStatus.Status = Status;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("Catch Exception\r\n");
				Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}
		case IOCTL_SYS_REMOVE_CALLBACK_ITEM:
		{
			DbgPrint("Remove Callback\r\n");

			if (InputBufferLength >= sizeof(PSYS_CALLBACK_ENTRY_INFORMATION) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT8));

					Status = RemoveCallbackNotify((PSYS_CALLBACK_ENTRY_INFORMATION)InputBuffer);

					Irp->IoStatus.Information = 0;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}
			break;
		}
		case IOCTL_SYS_ENUM_IOTIMER_LIST:
		{
			DbgPrint("Enum IoTimer\r\n");
			__try
			{
				ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

				Status = EnumIoTimer(OutputBuffer, OutputBufferLength);

				Irp->IoStatus.Information = 0;
				Irp->IoStatus.Status = Status;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("Catch Exception\r\n");
				Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}
		case IOCTL_SYS_REMOVE_IOTIMER_ITEM:
		{
			DbgPrint("Remove IoTimer\r\n");

			if (InputBufferLength >= sizeof(UINT_PTR) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT8));

					Status = RemoveIoTimer((PLIST_ENTRY)*(PUINT_PTR)InputBuffer);

					Irp->IoStatus.Information = 0;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}
			break;
		}
		case IOCTL_SYS_RUNORSTOP_IOTIMER_ITEM:
		{
			DbgPrint("Run Or Stop IoTimer\r\n");

			if (InputBufferLength >= sizeof(POPERATION_ON_IO_TIMER_INFORMATION) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT8));

					Status = RunOrStopIoTimer((POPERATION_ON_IO_TIMER_INFORMATION)InputBuffer);

					Irp->IoStatus.Information = 0;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}
			break;
		}
		case IOCTL_SYS_ENUM_DPCTIMER_LIST:
		{
			DbgPrint("Enum DpcTimer\r\n");
			__try
			{
				ProbeForWrite(OutputBuffer, OutputBufferLength, sizeof(UINT8));

				Status = EnumDpcTimer(OutputBuffer, OutputBufferLength);

				Irp->IoStatus.Information = 0;
				Irp->IoStatus.Status = Status;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("Catch Exception\r\n");
				Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}
		case IOCTL_SYS_REMOVE_DPCTIMER_ITEM:
		{
			DbgPrint("Remove DpcTimer\r\n");

			if (InputBufferLength >= sizeof(UINT_PTR) && InputBuffer)
			{
				__try
				{
					ProbeForRead(InputBuffer, InputBufferLength, sizeof(UINT8));

					Status = RemoveDpcTimer(*(PUINT_PTR)InputBuffer);

					Irp->IoStatus.Information = 0;
					Irp->IoStatus.Status = Status;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("Catch Exception\r\n");
					Status = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}
			break;
		}


		default:
			Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			break;
		}
		break;
	}
	default:
		break;
	}

	Status = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}