# Studio Launcher – dbghelp Setup

## If you have already used Studio before

1. Navigate to `Library/dbg/` in your Studio installation folder.
2. Replace the existing `dbghelp.dll` with the new one.

## If you are installing Studio for the first time

1. Go to `Library/dbg/`.
2. Rename the original `dbghelp.dll` to `dbghelp_r.dll`.
3. Place the new `dbghelp.dll` in the same folder (`Library/dbg/`).

> The launcher will load `dbghelp_r.dll` (the original) and also attempt to load `studio.dll` from the same folder where your executable resides.