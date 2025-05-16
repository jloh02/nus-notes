from pypdf import PdfWriter, PdfReader

def merge(files = [], output="output.pdf"):
  writer = PdfWriter()
  for file in files:
      reader = PdfReader(file)
      for page in reader.pages:
          writer.add_page(page)
  with open(output, "wb") as f_out:
      writer.write(f_out)

# merge(["3210/CS3210 Tut 2 (2024) (2).pdf","3210/CS3210-TL6 (Tut 3) (2).pdf","3210/Tut 5.pdf","3210/CS3210 TL10 (Tut 6 + Lab 6).pdf"], "CS3210 Tut.pdf")
# 1,3-18,20-22,24-25,29-34,36-40,42-50,53,56,71-73,76-88,90-96,98-110,113-114,122,125-127,130,134,136-140,144-152,161,163-164,170,175,177-181,183-186

# merge(["3210/L02-Processes-Threads-2024-E-Lecture-SelfStudy.pdf","3210/L03-Architecture-Memory.pdf","3210/L04-Parallel-Programming-Models-I-postlecture.pdf","3210/L05-Performance-postlecture-fixed.pdf","3210/L06-GPGPU.pdf","3210/L07-Coherence-Consistency.pdf","3210/L08-Parallel-Programming-Models-II-postlecture.pdf","3210/L09-Message-Passing.pdf","3210/L10-Interconnections.pdf","3210/L11-Energy-Efficient-Computing.pdf","3210/L12-Summary.pdf"],"CS3210 Lecs.pdf")
# 4,8-17,19-26,42,44,47-50,57,59,61-65,73-75,80,82,84-87,89-96,99-102,104-106,107-108,111,113-120,122-126,128,132-136,138,143-144,146,148,150-194,197,199-200,202-205,212-216,220-224,226-227,231-240,243-246,248-255,258,259,262,270-271,273,275-276,278-281,283-289,291-294,296-300,303,304,306,308-316,320,323,325-326,328,330,332-335,338,343-347,354-357,368,374,379,385-393,405-407,413-415,418-419,421,423-427,429,437-439,441-443,446,448-449,453-454,457-460,462-464,466-468,470-478,480,482-483,487-491,497-498,501,503-509,513,516,517,520,522,525-527,529-533,537-540,545-546,549,551,553-561,564,565,567-573,575-580,582,585-599,601-604,606-608,610-614,616-620,622-626,628-633,635-639,641-644,646-650,652-659

# 3,7-16,18-25,41,43,46-49,56,58,60-64,72-74,79,81,83-86,88-95,98-101,103-105,106-107,110,112-119,121-125,127,131-135,137,142-143,145,147,149-193,196,198-199,201-204,211-215,219-223,225-226,230-239,242-245,247-254,257,258,261,269-270,272,274-275,277-280,282-288,290-293,295-299,302,303,305,307-315,319,322,324-325,327,329,331-334,337,342-346,353-356,367,373,378,384-392,404-406,412-414,417-418,420,422-426,428,436-438,440-442,445,447-448,452-453,456-459,461-463,465-467,469-477,479,481-482,486-490,496-497,500,502-508,512,515,516,519,521,524-526,528-532,536-539,544-545,548,550,552-560,563,564,566-572,574-579,581,584-598

# 600-603,605-607,609-613,615-619,621-625,627-632,634-638,640-643,645-649,651-658


# merge([
# "3211/L1-Introduction.pdf",
# "3211/L2-Threads.pdf",
# "3211/L3-Synchronization.pdf",
# "3211/L4-Atomics.pdf",
# "3211/L4a-MM-Examples.pdf",
# "3211/L5-Concurrent_Data_Structures.pdf",
# "3211/L6-Debugging_Concurrent_Programs.pdf",
# "3211/L7-Concurrency-in-Go.pdf",
# "3211/L8-Concurrency_Patterns_in_Go.pdf",
# "3211/L9-Classical_Synchronization_Problems.pdf",
# "3211/L10-Safety_in_Rust.pdf",
# "3211/L11-Asynchronous_Programming_in_Rust.pdf",
# "3211/L12-Checking_Concurrent_Programs.pdf"
# ],


# "CS3211 Lec.pdf")
# 423-454,461,465-470,484-486,496-497,499,515-518
# 522-602

# merge([
#   "3211/T1-Threads-and-Synchronization-in-C++.pdf",
#   "3211/T2-Atomics-and-Memory-Model.pdf",
#   "3211/T3-Smart-Pointers.pdf",
#   "3211/T4-Lock-Free-Programming.pdf",
#   "3211/T5-Goroutines-And-Channels.pdf",
#   "3211/T6-Go-Concurrency-Patterns.pdf",
#   "3211/T7-Classic-Concurrency-Problems.pdf",
#   "3211/T7.5-Startup-in-Rust.pdf",
#   "3211/T8-Safety-and-Concurrency-in-Rust.pdf",
#   "3211/T9-Asynchronous-Programming-in-Rust.pdf",
# ],
# "CS3211 Tut.pdf")

# merge([
#   "3211/T7.5-Startup-in-Rust.pdf",
#   "3211/T8-Safety-and-Concurrency-in-Rust.pdf",
#   "3211/T9-Asynchronous-Programming-in-Rust.pdf",
# ],
# "CS3211 Tut - Rust.pdf")