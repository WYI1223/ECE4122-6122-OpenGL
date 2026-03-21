# PR-HW3-09: Ground, Tuning, README, And Submission Checks

- Proposed title: `chore(polish): finish rubric-facing details and submission readiness`
- Status: Draft

## Goal

Close out the homework with rubric-facing polish and submission checks:
- finish the ground presentation, including visible tiling
- tune object scale, rotation, and spacing for a coherent scene
- document build/use/asset choices in a homework README
- confirm the final package is clean and reproducible

This PR is intentionally late in the sequence because it depends on the scene
and lighting paths already being stable.

## Preconditions

- `PR-HW3-00` is complete
- `PR-HW3-08` is complete

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| scene owner | `src/main.cpp` | Owns terrain presentation, placement tuning, and final per-frame draw behavior |
| roadmap | `overall-pr-spec.md` | Defines the final rubric-aligned target |
| submission docs | homework README | Documents build steps, controls, and chosen assets |
| stage baseline | `pr/stage0-environment.md` | Defines the verified top-level configure/build/run flow |

## Scope

In scope:
- finish terrain/ground presentation so the ground requirement is visibly satisfied
- tune transforms, spacing, and scene composition for grading clarity
- confirm at least five distinct models are used and are arranged intentionally
- add or finalize the homework README
- verify that generated build artifacts are not part of the intended submission set

Out of scope:
- introducing new architectural subsystems
- adding extra-credit rendering features unless explicitly chosen later
- changing the Stage 0 environment baseline

## Design Constraints

- Preserve the fixed skeleton structure and the existing top-level integration flow.
- Treat the PDF rubric as the SSOT for what must be visibly true at the end.
- Prefer minimal, local polish changes over new systems.
- Keep submission documentation with the homework content, not as a redesign of the course repo.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | polish | Finalize the ground presentation and confirm visible tiling | `src/main.cpp` | PR-HW3-08 |
| T2 | polish | Tune object transforms so composition, rotation, and scale variety are obvious | `src/main.cpp` | T1 |
| T3 | docs | Add or finalize the homework README with build, controls, and asset notes | homework README | PR-HW3-08 |
| T4 | verify | Re-run clean-build and packaging checks before submission | top-level repo flow | T1-T3 |

## Planned File Changes

- `[edit]` `src/main.cpp`
- `[add/edit]` homework `README.md`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build_hmk3_stage0 --config Debug --target Hmk3_Skeleton
```

### Runtime verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL/build/bin
./Hmk3_Skeleton.exe
```

Expected result:
- the final scene reads clearly as a single farmyard or farmstead composition
- ground coverage is obvious and visibly tiled
- at least five distinct models are visible
- lighting, texturing, and scale choices look intentional rather than placeholder

### Submission verification

- build from a clean build directory still succeeds
- the README documents build steps, controls, and asset choices
- generated build artifacts are excluded from the intended submission set

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Scene technically works but looks cramped or random | Rubric points depend on visible composition quality | Reserve explicit time for transform tuning after functionality is stable |
| Ground exists but does not read as textured/intentional | The terrain requirement can still look unfinished | Make tiling and scale visually obvious in the final pass |
| README is left until the very end | Submission quality suffers even if the code runs | Treat README completion as part of the PR exit criteria |

## Acceptance Criteria

- [ ] The final scene visibly satisfies the composition and ground requirements
- [ ] A homework README is present with build steps, controls, and asset notes
- [ ] The project still builds cleanly from the validated top-level workflow
- [ ] The homework is in a submission-ready state without generated build artifacts
