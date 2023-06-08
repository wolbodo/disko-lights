<script lang="ts">
  export let programs: string[] = [];
  import { send } from './config'

</script>

<select disabled={!programs.length} on:change={({ target }) => send({ program: target.value }) }>
  {#each programs as program}
    <option value={program}>{program}</option>
  {:else}
    <option>Loading</option>
  {/each}
</select>