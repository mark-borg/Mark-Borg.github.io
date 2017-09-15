using System;
using System.Runtime.InteropServices;


namespace Validator
{
    public static class GenericMarshaller
    {
        public static IntPtr StringArrayToIntPtr<Char>(string[] InputStrArray) where Char : struct
        {
            int size = InputStrArray.Length;

            // build an array of pointers to string
            IntPtr[] ptrs = new IntPtr[size];

            int dim = IntPtr.Size * size;
            IntPtr rRoot = Marshal.AllocCoTaskMem(dim);

            for (int i = 0; i < size; ++i)
            {
                if (typeof(Char) == typeof(char))
                    ptrs[i] = Marshal.StringToCoTaskMemUni(InputStrArray[i]);
                else if (typeof(Char) == typeof(byte))
                    ptrs[i] = Marshal.StringToCoTaskMemAnsi(InputStrArray[i]);
                else if (typeof(Char) == typeof(IntPtr))         // assume BSTR for IntPtr param
                    ptrs[i] = Marshal.StringToBSTR(InputStrArray[i]);
            }

            // copy the array of pointers
            Marshal.Copy(ptrs, 0, rRoot, size);

            return rRoot;
        }

        public static string[] IntPtrToStringArray<Char>(int size, IntPtr rRoot) where Char : struct
        {
            // get the output array of pointers
            IntPtr[] ptrs = new IntPtr[size];
            Marshal.Copy(rRoot, ptrs, 0, size);

            string[] OutputStrArray = new string[size];
            for (int i = 0; i < size; ++i)
            {
                if (typeof(Char) == typeof(char))
                    OutputStrArray[i] = Marshal.PtrToStringUni(ptrs[i]);
                else if (typeof(Char) == typeof(byte))
                    OutputStrArray[i] = Marshal.PtrToStringAnsi(ptrs[i]);
                else if (typeof(Char) == typeof(IntPtr))        // assume BSTR for IntPtr param
                    OutputStrArray[i] = Marshal.PtrToStringBSTR(ptrs[i]);

                // dispose of unneeded memory
                Marshal.FreeCoTaskMem(ptrs[i]);
            }

            // dispose of the pointers array
            Marshal.FreeCoTaskMem(rRoot);
            return OutputStrArray;
        }

        public static IntPtr IntPtrFromStuctArray<T>(T[] InputArray) where T : new()
        {
            int size = InputArray.Length;
            T[] resArray = new T[size];

            int dim = IntPtr.Size * size;
            IntPtr rRoot = Marshal.AllocCoTaskMem(Marshal.SizeOf(InputArray[0]) * size);
            for (int i = 0; i < size; ++i)
                Marshal.StructureToPtr(InputArray[i], (IntPtr)(rRoot.ToInt32() + i * Marshal.SizeOf(InputArray[i])), false);

            return rRoot;
        }

        public static T[] StuctArrayFromIntPtr<T>(IntPtr outArray, int size) where T : new()
        {

            T[] resArray = new T[size];
            IntPtr current = outArray;
            for (int i = 0; i < size; ++i)
            {
                resArray[i] = new T();
                Marshal.PtrToStructure(current, resArray[i]);
                Marshal.DestroyStructure(current, typeof(T));
                int structsize = Marshal.SizeOf(resArray[i]);
                current = (IntPtr)((long)current + structsize);
            }
            Marshal.FreeCoTaskMem(outArray);
            return resArray;
        }
    }
}

