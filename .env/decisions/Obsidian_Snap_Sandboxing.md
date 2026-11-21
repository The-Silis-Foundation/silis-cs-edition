# Obsidian Snap Sandboxing Hindrance

## The Hindrance
When launching Obsidian via the command line URI callback:
```bash
snap run obsidian "obsidian://open?path=/home/econsystems/silisfoundation/docs/vault"
```
Obsidian booted into an empty dialog stating `"vault not found"`.

## Root Cause
Obsidian on this system is installed as an Ubuntu Snap package (`/snap/bin/obsidian`).
Snap applications run inside strict AppArmor and filesystem sandboxes:
- Snap does **not** read or write to the standard host configuration at `/home/econsystems/.config/obsidian/obsidian.json`.
- Instead, Snap redirects all configuration lookups to an isolated sandbox path:
  `/home/econsystems/snap/obsidian/current/.config/obsidian/obsidian.json`
- Because the newly created vault was not yet indexed with an assigned internal ID in the Snap-specific JSON file, URI callbacks failed to match the filesystem path.

## Resolution
1. Created the configuration directly inside the Snap sandbox:
   `/home/econsystems/snap/obsidian/current/.config/obsidian/obsidian.json`
2. Explicitly mapped the vault ID and path:
   ```json
   {
     "vaults": {
       "silis-vault-01": {
         "path": "/home/econsystems/silisfoundation/docs/vault",
         "ts": 1730718600000,
         "open": true
       }
     }
   }
   ```
3. Re-launched Obsidian passing the path directly: `snap run obsidian /home/econsystems/silisfoundation/docs/vault`.

## Related Notes
- [[00_Index]]
- [[decisions/Token_Lean_EDA_Investigation]]
